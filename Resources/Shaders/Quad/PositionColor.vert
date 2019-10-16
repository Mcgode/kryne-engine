#version 330 core

layout (location = 0) in vec3 inputPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 color;

void main() {
	gl_Position = projection * view * model * vec4(inputPosition, 1.0);
	color = 0.5 + 0.5 * normalize(inputPosition);
}
