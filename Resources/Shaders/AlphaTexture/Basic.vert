#version 330 core

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;
layout (location = 2) in vec2 inputUVCoordiates;
layout (location = 3) in vec3 inputTangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMat;

out VS_OUT {
	vec3 worldPosition;
	vec3 normal;
	vec2 textureCoordinates;
} vs_out;

void main() {
	vs_out.worldPosition = (model * vec4(inputPosition, 1.0)).xyz;
	vs_out.normal = normalMat * inputNormal;
	vs_out.textureCoordinates = inputUVCoordiates;

	gl_Position = projection * view * model * vec4(inputPosition, 1.0);
}
