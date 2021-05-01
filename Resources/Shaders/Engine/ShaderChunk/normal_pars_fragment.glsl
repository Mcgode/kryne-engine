
#ifdef USE_NORMAL_MAP

uniform sampler2D normalMap;

in vec3 vTangent;
in vec3 vBitangent;

vec3 getNormal( vec3 n )
{
    mat3 TBN = mat3(normalize(vTangent),
                    normalize(vBitangent),
                    normalize(n));

    return normalize(TBN * (2 * texture(normalMap, vUv).rgb - 1));
}

#else

vec3 getNormal( vec3 n )
{
    return normalize(n);
}

#endif
