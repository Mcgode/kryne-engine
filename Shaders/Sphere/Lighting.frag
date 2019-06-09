#version 330 core

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    int enabled;

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


out vec4 FragColor;

in vec3 worldPosition;
in vec3 normal;

uniform vec3 color;
uniform vec3 cameraPosition;
uniform DirectionalLight directionalLight;
uniform int amountOfPointLights = 0;
#define MAXIMUM_AMOUNT_OF_POINT_LIGHTS 4
uniform PointLight pointLights[MAXIMUM_AMOUNT_OF_POINT_LIGHTS];


vec3 getPointLightColor(PointLight light)
{
    vec3 norm = normalize(normal);
    vec3 l = normalize(light.position - worldPosition);
    float dist = length(worldPosition - light.position);
    float distFactor = 1 / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

    vec3 ambientColor = color * light.ambient * 0.1;

    float difFactor = max(0, dot(norm, l));
    vec3 diffuseColor = distFactor * difFactor * color * light.diffuse;

    int shininess = 32;
    vec3 r = reflect(l, norm);
    vec3 v = normalize(cameraPosition - worldPosition);
    float specFactor = pow(max(0, dot(r, v)), shininess);
    vec3 specularColor = distFactor * specFactor * color * light.specular;

    return ambientColor + diffuseColor + specularColor;
}


vec3 getDirectionalLightColor(DirectionalLight light)
{
    vec3 ambientColor = color * light.ambient * 0.1;

    vec3 l = normalize(-light.direction);
    float difFactor = max(0, dot(l, normalize(normal)));
    vec3 diffuseColor = difFactor * color * light.diffuse;

    float shininess = 32;
    vec3 r = reflect(l, normalize(normal));
    vec3 v = normalize(cameraPosition - worldPosition);
    float specFactor = pow(max(0, dot(r, v)), shininess);
    vec3 specularColor = specFactor * light.specular * color;

    return diffuseColor + ambientColor + specularColor;
}


void main() {
	vec3 finalColor = vec3(0.1);

	if (directionalLight.enabled != 0)
	    finalColor += getDirectionalLightColor(directionalLight);
	else
	    finalColor = color;

	for (int i = 0; i < MAXIMUM_AMOUNT_OF_POINT_LIGHTS && i < amountOfPointLights; i++)
	    finalColor += getPointLightColor(pointLights[i]);

	FragColor = vec4(finalColor, 1);
}
