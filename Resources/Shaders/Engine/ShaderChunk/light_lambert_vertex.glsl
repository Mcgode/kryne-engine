vLightDirectDiffuse = vec3(0);
vLightIndirectDiffuse = vec3(0);

GeometryData geometry;
geometry.worldPosition = worldPosition;
geometry.normal = normalize( normal );

DirectLightData directData;
IndirectLightData indirectData;

#ifdef MAX_DIRECTIONAL_LIGHTS

    #ifdef DYNAMIC_MAX_DIRECTIONAL_LIGHTS
    for ( int i = 0; i < maxDirectionalLights; i++ ) {
    #else
    for ( int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++ ) {
    #endif

        getDirectionalLightData( directionalLights[i], geometry, directData );
        vLightDirectDiffuse += max( 0, dot( -normalize( directData.direction ), geometry.normal ) ) * directData.color;

    }

#endif

    
#ifdef MAX_AMBIENT_LIGHTS

    #ifdef DYNAMIC_MAX_AMBIENT_LIGHTS
    for ( int i = 0; i < maxAmbientLights; i++ ) {
    #else
    for ( int i = 0; i < MAX_AMBIENT_LIGHTS; i++ ) {
    #endif

        getAmbientLightData( ambientLights[i], geometry, indirectData );
        vLightIndirectDiffuse += indirectData.color;

    }

#endif



#ifdef MAX_HEMISPHERE_LIGHTS

    #ifdef DYNAMIC_MAX_HEMISPHERE_LIGHTS
    for ( int i = 0; i < maxHemisphereLights; i++ ) {
    #else
    for ( int i = 0; i < MAX_HEMISPHERE_LIGHTS; i++ ) {
    #endif

        getHemisphereLightData( hemisphereLights[i], geometry, indirectData );
        vLightIndirectDiffuse += indirectData.color;
    
    }

#endif