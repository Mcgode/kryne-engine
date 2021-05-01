#include <vertex_pars_begin>

#include <common_structs>
#include <uv_pars_vertex>
#include <normal_pars_vertex>

out GeometryData vGeometry;

void main() {

    #include <vertex_begin>
    #include <vertex_project>

    #include <uv_vertex>
    #include <normal_vertex>

    GeometryData geometry;
    #include <vertex_geometry>
    vGeometry = geometry;

}
