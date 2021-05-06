#version 450

layout( set = 0, binding = 0 ) uniform UniformBufferObject {

    mat4 worldMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;

} ubo;

layout( location = 0 ) in vec2 position;
layout( location = 1 ) in vec3 color;

layout( location = 0 ) out vec3 fragColor;

void main() {

    gl_Position = ubo.projectionMatrix * ubo.viewMatrix * ubo.worldMatrix * vec4( position, 0, 1 );
    fragColor = color;

}