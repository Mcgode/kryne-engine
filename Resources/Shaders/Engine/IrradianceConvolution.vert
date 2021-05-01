uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 direction;

void main()
{
    direction = position;
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
}
