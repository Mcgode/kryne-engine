uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 direction;

void main()
{
    direction = position;
    vec4 pos = projectionMatrix * viewMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
