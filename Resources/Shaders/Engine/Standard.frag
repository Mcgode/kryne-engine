#include <common_structs>
#include <uv_pars_fragment>
#include <color_pars_fragment>

#define PI 3.14159265

in GeometryData vGeometry;

#include <bsdfs>
#include <physical_material_pars_fragment>

#include <lights_pars_begin>
#include <lights_physical_pars_fragment>
#include <normal_pars_fragment>

#include <envmap_pars_fragment>
#include <envmap_physical_pars_fragment>

out vec4 FragColor;

void main() {

    #include <color_fragment>

    GeometryData geometry = GeometryData(vGeometry.position,
                                         getNormal(vGeometry.normal),
                                         normalize(vGeometry.viewDir));

    PhysicalMaterial material = PhysicalMaterial( diffuseColor, roughness, metalness, vec3(0), vec3(0) );

    #ifdef USE_ROUGHNESS_MAP

        material.roughness *= texture(roughnessMap, vUv).r;

    #endif

    #ifdef USE_METALNESS_MAP

        material.metalness *= texture(metalnessMap, vUv).r;

    #endif

    material.diffuseColor = material.albedo * (1 - material.metalness);
    material.specularColor = mix(vec3(0.04), material.albedo, material.metalness);

    #include <lights_fragment>

    #ifdef USE_ENVMAP

        vec3 envMapIrradiance = GetEnvMapIndirectIrradiance(envMap, envMapIntensity, geometry.normal);
        vec3 envMapRadiance = GetEnvMapIndirectRadiance(envMap, envMapIntensity, geometry.viewDir, geometry.normal, material.roughness);

        ApplyIndirectSpecularLight(envMapRadiance, envMapIrradiance, geometry, material, reflectedLight);

    #endif

    vec3 outgoingLight = reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;

    FragColor = vec4( outgoingLight, opacity );

}
