#ifdef MAX_DIRECTIONAL_LIGHTS

    struct DirectionalLight {
        vec3 color;
        vec3 direction;
        uvec4 shadowMapIndexes;
    };

    uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

    #ifdef DYNAMIC_MAX_DIRECTIONAL_LIGHTS

        uniform int maxDirectionalLights;
        #define ITERATE_DIRECTIONAL_LIGHTS for ( int i = 0; i < maxDirectionalLights; i++ )

        #else

        #define ITERATE_DIRECTIONAL_LIGHTS for ( int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++ )

        #endif

    void getDirectionalLightData( const in DirectionalLight directionalLight,
                                  const in GeometryData geometry,
                                  out IncidentLight lightData ) {

        #ifdef USE_DIRECTIONAL_SHADOW_MAPS

            float shadow = getDirectionalShadow( directionalLight.shadowMapIndexes, geometry );

        #else

            float shadow = 1;

        #endif

        lightData.color = shadow * directionalLight.color;
        lightData.direction = -directionalLight.direction;

    }

#endif


#ifdef MAX_AMBIENT_LIGHTS

    struct AmbientLight {
        vec3 color;
    };

    uniform AmbientLight ambientLights[MAX_AMBIENT_LIGHTS];

    #ifdef DYNAMIC_MAX_AMBIENT_LIGHTS

        uniform int maxAmbientLights;
        #define ITERATE_AMBIENT_LIGHTS for ( int i = 0; i < maxAmbientLights; i++ )

    #else

        #define ITERATE_AMBIENT_LIGHTS for ( int i = 0; i < MAX_AMBIENT_LIGHTS; i++ )

    #endif

    vec3 getAmbientLightData( const in AmbientLight ambientLight,
                              const in GeometryData geometry ) {

        return ambientLight.color;

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
        #define ITERATE_HEMISPHERE_LIGHTS for ( int i = 0; i < maxHemisphereLights; i++ )

    #else

        #define ITERATE_HEMISPHERE_LIGHTS for ( int i = 0; i < MAX_HEMISPHERE_LIGHTS; i++ )

    #endif

    vec3 getHemisphereLightData( const in HemisphereLight hemisphereLight,
                                 const in GeometryData geometry ) {

        return mix( hemisphereLight.groundColor,
                    hemisphereLight.skyColor,
                    dot( geometry.normal, normalize( hemisphereLight.direction ) ) * 0.5 + 0.5 );

    }

#endif
