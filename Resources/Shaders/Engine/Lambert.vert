#include <vertex_pars_begin>

#include <common_structs>
#include <uv_pars_vertex>
#include <lights_pars_begin>

out ReflectedLight lambertLight;

void main() {

    #include <vertex_begin>
    #include <vertex_project>

    #include <uv_vertex>

    GeometryData geometry;
    #include <vertex_geometry>
    #include <lights_lambert_vertex>

}
