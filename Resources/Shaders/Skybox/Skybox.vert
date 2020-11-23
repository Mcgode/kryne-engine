

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;
layout (location = 2) in vec2 inputTextureCoordinates;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 direction;

void main() {
    direction = inputPosition;
	gl_Position = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(inputPosition, 1.0);
    gl_Position = gl_Position.xyww;
}
