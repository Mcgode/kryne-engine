out vec4 FragColor;

in vec3 direction;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, direction);
}
