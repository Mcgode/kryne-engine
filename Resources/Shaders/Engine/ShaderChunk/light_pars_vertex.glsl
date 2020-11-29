struct GeometryData {
    vec3 worldPosition;
    vec3 normal;
};

struct DirectLightData {
    vec3 color;
    vec3 direction;
};

struct IndirectLightData {
    vec3 color;
};

#ifdef MAX_DIRECTIONAL_LIGHTS

    struct DirectionalLight {
        vec3 color;
        vec3 direction;
    };

    uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

    #ifdef DYNAMIC_MAX_DIRECTIONAL_LIGHTS
        uniform int maxDirectionalLights;
    #endif

    void getDirectionalLightData( const in DirectionalLight directionalLight,
                                  const in GeometryData geometry,
                                  out DirectLightData lightData ) {

        lightData.color = directionalLight.color;
        lightData.direction = directionalLight.direction;

    }

    #define DIR_LIGHT_COUNT MAX_DIRECTIONAL_LIGHTS

#else

    #define DIR_LIGHT_COUNT 0

#endif


#ifdef MAX_AMBIENT_LIGHTS

    struct AmbientLight {
        vec3 color;
    };

    uniform AmbientLight ambientLights[MAX_AMBIENT_LIGHTS];

    #ifdef DYNAMIC_MAX_AMBIENT_LIGHTS
        uniform int maxAmbientLights;
    #endif

    void getAmbientLightData( const in AmbientLight ambientLight,
                              const in GeometryData geometry,
                              out IndirectLightData lightData ) {

        lightData.color = ambientLight.color;

    }

#endif


#ifdef MAX_HEMISPHERE_LIGHTS

    struct HemisphereLight {
        vec3 skyColor;
        vec3 groundColor;
        vec3 direction;
    };

    uniform HemisphereLight hemisphereLights[MAX_HEMISPHERE_LIGHTS];

    #ifdef DYNAMIC_MAX_HEMISPHERE_LIGHTS
        uniform int maxHemisphereLights;
    #endif

    void getHemisphereLightData( const in HemisphereLight hemisphereLight,
                                 const in GeometryData geometry,
                                 out IndirectLightData lightData ) {

        lightData.color = mix( hemisphereLight.groundColor,
                               hemisphereLight.skyColor,
                               dot( geometry.normal, normalize( hemisphereLight.direction ) ) * 0.5 + 0.5 );

    }

#endif


#define DIRECT_LIGHT_COUNT DIR_LIGHT_COUNT
