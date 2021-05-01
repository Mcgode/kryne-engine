
vec3 diffuseColor = color;
float opacity = 1;

#ifdef USE_MAP

    vec4 mapData = texture(map, vUv);
    diffuseColor *= mapData.rgb;
    opacity *= mapData.a;

#endif
