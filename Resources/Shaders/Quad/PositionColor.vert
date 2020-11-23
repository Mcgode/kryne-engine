

layout (location = 0) in vec3 inputPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 color;

void main() {
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inputPosition, 1.0);
	color = 0.5 + 0.5 * normalize(inputPosition);
}
