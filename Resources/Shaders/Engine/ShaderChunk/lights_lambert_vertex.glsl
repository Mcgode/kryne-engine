lambertLight = ReflectedLight(vec3(0), vec3(0), vec3(0), vec3(0));

IncidentLight directData;

#ifdef MAX_DIRECTIONAL_LIGHTS

    ITERATE_DIRECTIONAL_LIGHTS
    {

        getDirectionalLightData( directionalLights[i], geometry, directData );
        lambertLight.directDiffuse += max( 0, dot( -normalize( directData.direction ), geometry.normal ) ) * directData.color;

    }

#endif

    
#ifdef MAX_AMBIENT_LIGHTS

    ITERATE_AMBIENT_LIGHTS
    {

        lambertLight.indirectDiffuse += getAmbientLightData( ambientLights[i], geometry, indirectData );

    }

#endif



#ifdef MAX_HEMISPHERE_LIGHTS

    ITERATE_HEMISPHERE_LIGHTS
    {

        lambertLight.indirectDiffuse += getHemisphereLightData( hemisphereLights[i], geometry );
    
    }

#endif