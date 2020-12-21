
float opacity = 1;

#ifndef USE_MAP

    vec3 diffuseColor = color;

#else

    vec4 mapData = texture(map, vUv);
    vec3 diffuseColor = mapData.rgb;
    opacity *= mapData.a;

#endif
