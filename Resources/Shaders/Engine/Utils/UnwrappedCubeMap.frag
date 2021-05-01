uniform samplerCube cubeMap;
uniform float mipLevel;

in vec2 vUv;

out vec4 FragColor;

const float f1_3 = 1.f / 3.f;
const float f2_3 = 2.f / 3.f;

const vec2 size = vec2(0.25, f1_3);


vec3 coordsPX(vec2 v)
{
    vec2 localCoords = (v - vec2(2, 1) * size) / size;
    localCoords = localCoords * 2 - 1;
    vec2 tmpV = localCoords * vec2(-1, -1);
    return vec3(1, tmpV.yx);
}


vec3 coordsPY(vec2 v)
{
    vec2 localCoords = (v - vec2(1, 0) * size) / size;
    localCoords = localCoords * 2 - 1;
    vec2 tmpV = localCoords * vec2(1, 1);
    return vec3(tmpV.x, 1, tmpV.y);
}


vec3 coordsPZ(vec2 v)
{
    vec2 localCoords = (v - vec2(1, 1) * size) / size;
    localCoords = localCoords * 2 - 1;
    vec2 tmpV = localCoords * vec2(1, -1);
    return vec3(tmpV, 1);
}


vec3 coordsNX(vec2 v)
{
    vec2 localCoords = (v - vec2(0, 1) * size) / size;
    localCoords = localCoords * 2 - 1;
    vec2 tmpV = localCoords * vec2(1, -1);
    return vec3(-1, tmpV.yx);
}


vec3 coordsNY(vec2 v)
{
    vec2 localCoords = (v - vec2(1, 2) * size) / size;
    localCoords = localCoords * 2 - 1;
    vec2 tmpV = localCoords * vec2(1, 1);
    return vec3(tmpV.x, -1, tmpV.y);
}


vec3 coordsNZ(vec2 v)
{
    vec2 localCoords = (v - vec2(3, 1) * size) / size;
    localCoords = localCoords * 2 - 1;
    vec2 tmpV = localCoords * vec2(-1, -1);
    return vec3(tmpV, -1);
}


void main()
{
    bool firstQuart  = vUv.x <= 0.25;
    bool secondQuart = vUv.x <= 0.5 && !firstQuart;
    bool thirdQuart  = vUv.x <= 0.75 && !firstQuart && !secondQuart;
    bool fourthQuart = vUv.x >  0.75;

    bool up     = vUv.y <= f1_3;
    bool down   = vUv.y >  f2_3;
    bool middle = !(down || up);

    if (!middle && !secondQuart)
        discard;

    vec3 coords;

    if (firstQuart)
        coords = coordsNX(vUv);
    else if (secondQuart)
    {
        if (up)
            coords = coordsPY(vUv);
        else if (middle)
            coords = coordsPZ(vUv);
        else
            coords = coordsNY(vUv);
    }
    else if (thirdQuart)
        coords = coordsPX(vUv);
    else
        coords = coordsNZ(vUv);

    FragColor = textureLod(cubeMap, coords, mipLevel);
}