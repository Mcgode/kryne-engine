#version 330 core

#define MAX_AMOUNT_OF_POINT_LIGHTS 4
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


struct Material {
	float shininess;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	sampler2D diffuseMap;
	sampler2D normalMap;
};

out vec4 FragColor;

in VS_OUT {
	vec3 worldPosition;
	vec3 normal;
	vec2 textureCoordinates;
} fs_in;

uniform vec3 cameraPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform int amountOfPointLights = 0;
uniform PointLight pointLights[MAX_AMOUNT_OF_POINT_LIGHTS];
uniform float subsurfaceScattering = 0.8;

vec4 getPointLightColor(PointLight light)
{
	vec3 norm = normalize(fs_in.normal);
	vec3 l = normalize(light.position - fs_in.worldPosition);
	float dist = length(fs_in.worldPosition - light.position);
	float distFactor = 1 / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

	vec3 color = texture(material.diffuseMap, fs_in.textureCoordinates).xyz;

	vec3 ambientColor = color * material.ambient * light.ambient;

	float difFactor = max(0, dot(norm, l));
	difFactor += max(0, subsurfaceScattering  * dot(l, -norm));
	vec3 diffuseColor = distFactor * difFactor * color * material.diffuse * light.diffuse;

	float shininess = material.shininess;
	vec3 r = reflect(l, norm);
	vec3 v = normalize(cameraPosition - fs_in.worldPosition);
	float specFactor = pow(max(0, dot(r, v)), shininess);
	vec3 specularColor = specFactor * light.specular * color * material.specular;

	return vec4(ambientColor + diffuseColor + specularColor, specFactor);
}


vec4 getDirectionalLightColor(DirectionalLight light)
{
	vec3 color = texture(material.diffuseMap, fs_in.textureCoordinates).xyz;

	vec3 ambientColor = color * material.ambient * light.ambient;

	vec3 l = normalize(-light.direction);
	float difFactor = max(0, dot(l, normalize(fs_in.normal)));
	difFactor += max(0, subsurfaceScattering  * dot(l, -normalize(fs_in.normal)));
	vec3 diffuseColor = difFactor * color * material.diffuse * light.diffuse;

	float shininess = material.shininess;
	vec3 r = reflect(-l, normalize(fs_in.normal));
	vec3 v = normalize(cameraPosition - fs_in.worldPosition);
	float specFactor = pow(max(0, dot(r, v)), shininess);
	vec3 specularColor = specFactor * light.specular * color * material.specular;

	return vec4(diffuseColor + ambientColor + specularColor, specFactor);
}

void main() {
	vec4 finalColor = vec4(0, 0, 0, texture(material.diffuseMap, fs_in.textureCoordinates).a);

	if (directionalLight.enabled != 0)
		finalColor += getDirectionalLightColor(directionalLight);
	else
		finalColor = texture(material.diffuseMap, fs_in.textureCoordinates).rgba;

	for (int i = 0; i < MAX_AMOUNT_OF_POINT_LIGHTS && i < amountOfPointLights; i++)
		finalColor += getPointLightColor(pointLights[i]);

	FragColor = finalColor;
}
