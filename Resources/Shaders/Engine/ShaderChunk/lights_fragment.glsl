ReflectedLight reflectedLight;
IncidentLight lightData;

#ifdef MAX_DIRECTIONAL_LIGHTS

    ITERATE_DIRECTIONAL_LIGHTS
    {
        getDirectionalLightData( directionalLights[i], geometry, lightData );
        ApplyDirectLight( lightData, geometry, material, reflectedLight );
    }

#endif

    
#ifdef MAX_AMBIENT_LIGHTS

    ITERATE_AMBIENT_LIGHTS
    {
        ApplyIndirectDiffuseLight( getAmbientLightData( ambientLights[i], geometry ), geometry, material, reflectedLight );
    }

#endif



#ifdef MAX_HEMISPHERE_LIGHTS

    ITERATE_HEMISPHERE_LIGHTS
    {
        ApplyIndirectDiffuseLight( getHemisphereLightData( hemisphereLights[i], geometry ), geometry, material, reflectedLight );
    }

#endif