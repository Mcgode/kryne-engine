#version 330 core

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;
layout (location = 2) in vec2 inputTextureCoordinates;

uniform mat4 projection;
uniform mat4 view;

out vec3 direction;

void main() {
    direction = inputPosition;
	gl_Position = projection * mat4(mat3(view)) * vec4(inputPosition, 1.0);
    gl_Position = gl_Position.xyww;
}
