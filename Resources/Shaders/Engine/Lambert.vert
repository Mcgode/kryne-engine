#include <vertex_pars_begin>

#include <common_structs>
#include <lights_pars_begin>

out ReflectedLight lambertLight;

void main() {

    #include <vertex_begin>
    #include <vertex_project>

    GeometryData geometry;
    #include <vertex_geometry>
    #include <lights_lambert_vertex>

}
