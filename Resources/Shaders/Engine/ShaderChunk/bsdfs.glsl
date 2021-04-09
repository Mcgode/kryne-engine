

vec3 fresnelSchlick( const in float NdV, const in vec3 specularColor )
{
    // Original approximation by Christophe Schlick '94
    // float fresnel = pow( 1.0 - cosTheta, 5.0 );

    // Optimized variant (presented by Epic at SIGGRAPH '13)
    // https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
    float fresnel = exp2( ( -5.55473 * NdV - 6.98316 ) * NdV );

    return specularColor + (1.0 - specularColor) * fresnel;
}


// Microfacet Models for Refraction through Rough Surfaces - equation (33)
// http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
// alpha is "roughness squared" in Disney’s reparameterization
float DistributionGGX( const in float NdH, const in float alpha )
{
    float a2     = alpha * alpha;
    float NdotH2 = NdH * NdH;

    float denom = NdotH2 * (a2 - 1.0) + 1.0;

    return a2 / (PI * denom * denom);
}


// Microfacet Models for Refraction through Rough Surfaces - equation (34)
// http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
// alpha is "roughness squared" in Disney’s reparameterization
float GeometrySmithGGX( const in float NdV, const in float NdL, const in float alpha )
{
    float a2 = alpha * alpha;

    float gl = NdL + sqrt( a2 + ( 1.0 - a2 ) * NdL * NdL );
    float gv = NdV + sqrt( a2 + ( 1.0 - a2 ) * NdV * NdV );

    return 1.0 / (gl * gv);
}


// Moving Frostbite to Physically Based Rendering 3.0 - page 12, listing 2
// https://seblagarde.files.wordpress.com/2015/07/course_notes_moving_frostbite_to_pbr_v32.pdf
float GeometrySmithGGXCorrelated( const in float NdV, const in float NdL, const in float alpha )
{
    float a2 = alpha * alpha;

    // NdL and NdV are explicitly swapped. This is not a mistake.
    float gl = NdV + sqrt( a2 + ( 1.0 - a2 ) * NdL * NdL );
    float gv = NdL + sqrt( a2 + ( 1.0 - a2 ) * NdV * NdV );

    return 0.5 / max( gv + gl, 0.001 );
}


vec3 BRDFSpecularGGX(const in vec3 lightDirection,
                     const in vec3 viewDir,
                     const in vec3 normal,
                     const in vec3 specularColor,
                     const in float roughness)
{
    vec3 halfDir = normalize(lightDirection + viewDir);

    float alpha    = roughness * roughness;
    float cosTheta = max(0, dot(normal, lightDirection));

    float NdV = max(0, dot(normal, viewDir));
    float NdL = max(0, dot(normal, lightDirection));
    float NdH = max(0, dot(normal, halfDir));
    float LdH = max(0, dot(halfDir, viewDir));

    vec3 F = fresnelSchlick(LdH, specularColor);

    float NDF = DistributionGGX(NdH, alpha);
    float G   = GeometrySmithGGXCorrelated(NdV, NdL, alpha);

    return NDF * G * F;
}


// Analytical approximation of the DFG LUT, one half of the
// split-sum approximation used in indirect specular lighting.
// via 'environmentBRDF' from "Physically Based Shading on Mobile"
// https://www.unrealengine.com/blog/physically-based-shading-on-mobile - environmentBRDF for GGX on mobile
vec2 integrateSpecularBRDF(const in float dotNV, const in float roughness)
{
    const vec4 c0 = vec4(-1, -0.0275, -0.572, 0.022);

    const vec4 c1 = vec4(1, 0.0425, 1.04, - 0.04);

    vec4 r = roughness * c0 + c1;

    float a004 = min(r.x * r.x, exp2(-9.28 * dotNV)) * r.x + r.y;

    return vec2(-1.04, 1.04) * a004 + r.zw;

}


vec3 ApplyGGXSpecularBRDF(const in vec3 normal, const in vec3 viewDir, const in vec3 specularColor, const in float roughness)
{
    float NdV = clamp(dot(normal, viewDir), 0, 1);
    vec2 brdf = integrateSpecularBRDF(NdV, roughness);
    return specularColor * brdf.x + brdf.y;
}