

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMat;

out vec3 normal;
out vec3 worldPosition;

void main() {
    worldPosition = (modelMatrix * vec4(inputPosition, 1)).xyz;
    normal = normalMat * inputNormal;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inputPosition, 1.0);
}
