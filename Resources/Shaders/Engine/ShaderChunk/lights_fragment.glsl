ReflectedLight reflectedLight;
IncidentLight lightData;

#ifdef MAX_DIRECTIONAL_LIGHTS

    ITERATE_DIRECTIONAL_LIGHTS
    {

        getDirectionalLightData( directionalLights[i], geometry, lightData );
        ApplyDirectLight( lightData, geometry, material, reflectedLight );

    }

#endif

    
//#ifdef MAX_AMBIENT_LIGHTS
//
//    ITERATE_AMBIENT_LIGHTS
//    {
//
//        lambertLight.indirectDiffuse += getAmbientLightData( ambientLights[i], geometry, indirectData );
//
//    }
//
//#endif
//
//
//
//#ifdef MAX_HEMISPHERE_LIGHTS
//
//    ITERATE_HEMISPHERE_LIGHTS
//    {
//
//        lambertLight.indirectDiffuse += getHemisphereLightData( hemisphereLights[i], geometry );
//
//    }
//
//#endif