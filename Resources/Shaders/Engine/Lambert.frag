#include <common_structs>
#include <uv_pars_fragment>
#include <color_pars_fragment>

in ReflectedLight lambertLight;

out vec4 FragColor;

void main() {

    #include <color_fragment>

    vec3 outgoingLight = lambertLight.directDiffuse + lambertLight.indirectDiffuse;

    FragColor = vec4( outgoingLight * diffuseColor, opacity );

}
