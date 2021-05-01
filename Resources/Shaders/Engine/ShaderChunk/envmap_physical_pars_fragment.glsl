

vec3 GetEnvMapIndirectIrradiance(const in samplerCube envMap,
                                 const in float intensity,
                                 const in vec3 normal)
{
    return PI * textureLod(envMap, normal, 4.0).rgb * intensity;
}


vec3 GetEnvMapIndirectRadiance(const in samplerCube envMap,
                               const in float intensity,
                               const in vec3 viewDir,
                               const in vec3 normal,
                               const in float roughness)
{
    vec3 reflectVec = reflect(-viewDir, normal);

    // Mixing the reflection with the normal is more accurate and keeps rough objects from gathering light from behind their tangent plane.
    reflectVec = normalize( mix( reflectVec, normal, roughness * roughness) );

    float maxLod = 4;
    float sigma = PI * roughness * roughness / ( 1.0 + roughness );
    float desiredMIPLevel = maxLod + log2( sigma );

    return intensity * textureLod(envMap, reflectVec, clamp( desiredMIPLevel, 0.0, maxLod )).rgb;
}


