layout (location = 0) in vec3 inputPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    vec3 transformed = inputPosition;
    #include <vertex_position>
}