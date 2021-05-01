struct GeometryData {
    vec3 position;
    vec3 normal;
    vec3 viewDir;
};

struct IncidentLight {
    vec3 color;
    vec3 direction;
};

struct ReflectedLight {
    vec3 directDiffuse;
    vec3 indirectDiffuse;
    vec3 directSpecular;
    vec3 indirectSpecular;
};
