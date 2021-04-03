
struct PhysicalMaterial
{
    vec3 albedo;
    float roughness;
    float metalness;
};


vec3 fresnelSchlick( const in float cosTheta, const in vec3 F0 )
{
    // Original approximation by Christophe Schlick '94
    // float fresnel = pow( 1.0 - cosTheta, 5.0 );

    // Optimized variant (presented by Epic at SIGGRAPH '13)
    // https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
    float fresnel = exp2( ( -5.55473 * cosTheta - 6.98316 ) * cosTheta );

    return F0 + (1.0 - F0) * fresnel;
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


void LightDirectPhysical( const in IncidentLight light,
                          const in GeometryData geometry,
                          const in PhysicalMaterial material,
                          inout ReflectedLight reflectedLight )
{
    vec3 halfDir = normalize(light.direction + geometry.viewDir);

    float alpha    = material.roughness * material.roughness;
    float cosTheta = max(0, dot(geometry.normal, light.direction));

    float NdV = max(0, dot(geometry.normal, geometry.viewDir));
    float NdL = max(0, dot(geometry.normal, light.direction));
    float NdH = max(0, dot(geometry.normal, halfDir));
    float LdH = max(0, dot(halfDir, geometry.viewDir));

    vec3 F0 = mix(vec3(0.04), material.albedo, material.metalness);
    vec3 F  = fresnelSchlick(LdH, F0);

    float NDF = DistributionGGX(NdH, alpha);
    float G   = GeometrySmithGGXCorrelated(NdV, NdL, alpha);

    vec3 specular = NDF * G * F;

    vec3 kS = F;
    vec3 kD = (1 - kS) * (1 - material.metalness);

    reflectedLight.directDiffuse  += kD * material.albedo * light.color * cosTheta;
    reflectedLight.directSpecular += specular * light.color * cosTheta;
}


void LightIndirectDiffusePhysical( const in vec3 irradiance,
                                   const in GeometryData geometry,
                                   const in PhysicalMaterial material,
                                   inout ReflectedLight reflectedLight  )
{
    reflectedLight.indirectDiffuse += irradiance * material.albedo / PI;
}


#define ApplyDirectLight            LightDirectPhysical
#define ApplyIndirectDiffuseLight   LightIndirectDiffusePhysical
