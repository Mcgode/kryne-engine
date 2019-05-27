#version 330 core

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;
layout (location = 2) in vec2 inputTextureCoordinates;
layout (location = 3) in vec3 inputTangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMat;

out mat3 normalSpace;
out vec3 worldPosition;
out vec2 textureCoordinates;

void main() {
    worldPosition = (model * vec4(inputPosition, 1)).xyz;

    vec3 N = normalize(normalMat * inputNormal);
    vec3 T = normalize(normalMat * inputTangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    normalSpace = mat3(T, B, N);

    textureCoordinates = inputTextureCoordinates;

	gl_Position = projection * view * model * vec4(inputPosition, 1.0);
}
