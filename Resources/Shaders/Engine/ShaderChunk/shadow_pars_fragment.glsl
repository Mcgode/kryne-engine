
#ifdef MAX_DIRECTIONAL_SHADOW_MAPS

    #define USE_DIRECTIONAL_SHADOW_MAPS

    uniform mat4 directionalShadowMatrices[ MAX_DIRECTIONAL_SHADOW_MAPS ];
    uniform sampler2D directionalShadowMaps[ MAX_DIRECTIONAL_SHADOW_MAPS ];

    float getDirectionalShadow( const in uvec4 shadowMapIndexes, const in GeometryData geometry ) {

        float shadow = 1;
        for ( int i = 0; i < 4; i++ ) {

            if ( shadowMapIndexes[ i ] != 0 ) {

                uint index = shadowMapIndexes[ i ] - 1;

                vec4 shadowCoords = directionalShadowMatrices[ index ] * vec4(geometry.position, 1);
                shadowCoords /= shadowCoords.w;
                shadowCoords = 0.5 * shadowCoords + 0.5;

                if ( shadowCoords.x >= 0 && shadowCoords.y >= 0 && shadowCoords.x <= 1 && shadowCoords.y <= 1 ) {

                    float shadowDepth = texture(directionalShadowMaps[ index ], shadowCoords.xy).r;
                    shadow = ( shadowCoords.z < shadowDepth + 0.001 ) ? 1 : 0;
                    return shadow;

                }

            } else
                return shadow;
        }

        return shadow;

    }


#endif