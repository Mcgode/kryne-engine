#include <common_structs>
#include <uv_pars_fragment>
#include <color_pars_fragment>

#define PI 3.1415

in GeometryData vGeometry;

#include <physical_material_pars_fragment>

#include <lights_pars_begin>
#include <lights_physical_pars_fragment>
#include <normal_pars_fragment>

out vec4 FragColor;

void main() {

    #include <color_fragment>

    GeometryData geometry = GeometryData(vGeometry.position,
                                         getNormal(vGeometry.normal),
                                         normalize(vGeometry.viewDir));

    PhysicalMaterial material = PhysicalMaterial( diffuseColor, roughness, metalness );

    #ifdef USE_ROUGHNESS_MAP

        material.roughness *= texture(roughnessMap, vUv).r;

    #endif

    #ifdef USE_METALNESS_MAP

        material.metalness *= texture(metalnessMap, vUv).r;

    #endif

    #include <lights_fragment>

    vec3 outgoingLight = reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse;

    FragColor = vec4( outgoingLight, opacity );

}
