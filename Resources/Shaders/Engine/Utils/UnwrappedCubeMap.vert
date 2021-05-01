uniform mat4 matrix;
uniform bool invertVertical;

out vec2 vUv;


void main()
{
    vUv = invertVertical ? vec2(uv.x, 1 - uv.y) : uv;
    gl_Position = matrix * vec4(position, 1);
}