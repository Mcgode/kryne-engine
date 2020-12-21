
struct PhysicalMaterial
{
    vec3 albedo;
    float roughness;
    float metalness;
};


vec3 fresnelSchlick( const in float cosTheta, const in vec3 F0 )
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


float DistributionGGX( const in vec3 N, const in vec3 H, const in float roughness )
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}


float GeometrySchlickGGX( const in float NdotV, const in float roughness )
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}


float GeometrySmith( const in float NdV, const in float NdL, const in float roughness )
{
    float ggx2  = GeometrySchlickGGX(NdV, roughness);
    float ggx1  = GeometrySchlickGGX(NdL, roughness);

    return ggx1 * ggx2;
}


void LightDirectPhysical( const in IncidentLight light,
                          const in GeometryData geometry,
                          const in PhysicalMaterial material,
                          inout ReflectedLight reflectedLight )
{
    vec3 h = normalize(light.direction + geometry.viewDir);

    float cosTheta = max(0, dot(geometry.normal, light.direction));

    vec3 F0 = mix(vec3(0.04), material.albedo, material.metalness);
    vec3 F  = fresnelSchlick(max(0, dot(h, geometry.viewDir)), F0);

    float NdV = max(0, dot(geometry.normal, geometry.viewDir));
    float NdL = max(0, dot(geometry.normal, light.direction));

    float NDF = DistributionGGX(geometry.normal, h, material.roughness);
    float G   = GeometrySmith(NdV, NdL, material.roughness);

    vec3 num      = NDF * G * F;
    float denom   = 4.0 * NdV * NdL;
    vec3 specular = num / max(denom, 0.001);

    vec3 kS = F;
    vec3 kD = (1 - kS) * (1 - material.metalness);

    reflectedLight.directDiffuse  += kD * material.albedo * light.color * cosTheta;
    reflectedLight.directSpecular += specular * light.color * cosTheta;
}

#define ApplyDirectLight LightDirectPhysical
