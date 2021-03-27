in vec2 vUv;

out vec4 FragColor;

uniform sampler2D tDiffuse;

void main()
{
    FragColor = texture(tDiffuse, vUv);
}
