

uniform samplerCube cubemap;
uniform vec3 sunDirection;
uniform float angle = 0;

in vec3 direction;

out vec4 FragColor;

void main() {
	if (1.0 - dot(normalize(direction), normalize(sunDirection)) < angle)
        FragColor = vec4(1);
    else
        FragColor = texture(cubemap, direction);
}
