#version 330 core

#define MAX_AMOUNT_OF_POINT_LIGHTS 4
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    int enabled;

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


#define MAX_AMOUNT_OF_SHADOW_MAPS 5
struct ShadowMap {
    sampler2D shadowMap;
    mat4 lightSpaceMatrix;
    float shadowBias;
};

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;
layout (location = 2) in vec2 inputTextureCoordinates;
layout (location = 3) in vec3 inputTangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMat;
uniform vec3 cameraPosition;
uniform DirectionalLight directionalLight;
uniform int amountOfPointLights = 0;
uniform PointLight pointLights[MAX_AMOUNT_OF_POINT_LIGHTS];
uniform int amountOfShadowMaps = 0;
uniform ShadowMap directionalShadowMaps[MAX_AMOUNT_OF_SHADOW_MAPS];

out VS_OUT {
    vec3 worldPosition;
    vec2 textureCoordinates;
    vec3 tangeantWorldPosition;
    vec3 directionalLightDirection;
    vec3 cameraPosition;
    vec3 pointLightsPositions[MAX_AMOUNT_OF_POINT_LIGHTS];
    vec4 lightSpaceDirectionalPositions[MAX_AMOUNT_OF_SHADOW_MAPS];
} vs_out;

void main() {
    vs_out.worldPosition = (model * vec4(inputPosition, 1)).xyz;

    vec3 N = normalize(normalMat * inputNormal);
    vec3 T = normalize(normalMat * inputTangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.tangeantWorldPosition = TBN * vec3(model * vec4(inputPosition, 1));

    vs_out.textureCoordinates = inputTextureCoordinates;

    vs_out.cameraPosition = TBN * cameraPosition;

    vs_out.directionalLightDirection = TBN * directionalLight.direction;

    for (int i = 0; i < amountOfPointLights; i++) {
        vs_out.pointLightsPositions[i] = TBN * pointLights[i].position;
    }

    for (int i = 0; i < amountOfShadowMaps; i++)
        vs_out.lightSpaceDirectionalPositions[i] = directionalShadowMaps[i].lightSpaceMatrix * vec4(vs_out.worldPosition, 1.0);

	gl_Position = projection * view * model * vec4(inputPosition, 1.0);
}
