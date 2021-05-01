
#ifdef USE_NORMAL_MAP

    vTangent = normalize(tangent - dot(normal, tangent));
    vBitangent = cross(normal, tangent);

#endif
