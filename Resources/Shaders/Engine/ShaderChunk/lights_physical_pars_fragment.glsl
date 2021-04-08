
struct PhysicalMaterial
{
    vec3 albedo;
    float roughness;
    float metalness;
    vec3 diffuseColor;
    vec3 specularColor;
};


void LightDirectPhysical( const in IncidentLight light,
                          const in GeometryData geometry,
                          const in PhysicalMaterial material,
                          inout ReflectedLight reflectedLight )
{
    vec3 irradiance = max(0, dot(geometry.normal, light.direction)) * light.color;

    reflectedLight.directDiffuse  += irradiance * material.diffuseColor;
    reflectedLight.directSpecular += irradiance * BRDFSpecularGGX(light.direction, geometry.viewDir, geometry.normal, material.specularColor, material.roughness);
}


void LightIndirectDiffusePhysical( const in vec3 irradiance,
                                   const in GeometryData geometry,
                                   const in PhysicalMaterial material,
                                   inout ReflectedLight reflectedLight  )
{
    reflectedLight.indirectDiffuse += irradiance * material.diffuseColor / PI;
}


void LightIndirectSpecularPhysical(const in vec3 radiance,
                                   const in vec3 irradiance,
                                   const in GeometryData geometry,
                                   const in PhysicalMaterial material,
                                   inout ReflectedLight reflectedLight)
{
    reflectedLight.indirectSpecular += ApplyGGXSpecularBRDF(geometry.normal, geometry.viewDir, material.specularColor, material.roughness);
}


#define ApplyDirectLight            LightDirectPhysical
#define ApplyIndirectDiffuseLight   LightIndirectDiffusePhysical
#define ApplyIndirectSpecularLight  LightIndirectSpecularPhysical
