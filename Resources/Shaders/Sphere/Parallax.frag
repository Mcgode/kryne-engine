

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
    sampler2D depthMap;
};


out vec4 FragColor;

in VS_OUT {
    vec3 worldPosition;
    vec2 textureCoordinates;
    vec3 tangeantWorldPosition;
    vec3 directionalLightDirection;
    vec3 cameraPosition;
    vec3 pointLightsPositions[MAX_AMOUNT_OF_POINT_LIGHTS];
} fs_in;


in PointLight pointLightsPass[MAX_AMOUNT_OF_POINT_LIGHTS];

uniform Material material;
uniform DirectionalLight directionalLight;
uniform int amountOfPointLights = 0;
uniform PointLight pointLights[MAX_AMOUNT_OF_POINT_LIGHTS];
uniform float heightScale = 0.3;


float getDepth(vec2 coords) {
    return texture(material.depthMap, coords).r;
}


vec2 getParallaxMappingCoordinates(vec2 texCoords, vec3 viewDir)
{
    // number of depth layers
    float minLayers = 8;
    float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = getDepth(currentTexCoords);

    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = getDepth(currentTexCoords);
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = getDepth(prevTexCoords) - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}


vec3 getPointLightColor(PointLight light, vec3 position, vec3 normal, vec2 texCoords)
{
    vec3 norm = normal;
    vec3 l = normalize(position - fs_in.tangeantWorldPosition);
    float dist = length(fs_in.worldPosition - light.position);
    float distFactor = 1 / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

    vec3 color = texture(material.diffuseMap, texCoords).xyz;

    vec3 ambientColor = color * material.ambient * light.ambient;

    float difFactor = max(0, dot(norm, l));
    vec3 diffuseColor = distFactor * difFactor * color * material.diffuse * light.diffuse;

    float shininess = material.shininess;
    vec3 r = reflect(l, norm);
    vec3 v = normalize(fs_in.cameraPosition - fs_in.tangeantWorldPosition);
    float specFactor = pow(max(0, dot(r, v)), shininess);
    vec3 specularColor = specFactor * light.specular * color * material.specular;

    return ambientColor + diffuseColor + specularColor;
}


vec3 getDirectionalLightColor(DirectionalLight light, vec3 direction, vec3 normal, vec2 texCoords)
{
    vec3 color = texture(material.diffuseMap, texCoords).xyz;

    vec3 ambientColor = color * material.ambient * light.ambient;

    vec3 l = normalize(-direction);
    float difFactor = max(0, dot(l, normalize(normal)));
    vec3 diffuseColor = difFactor * color * material.diffuse * light.diffuse;

    float shininess = material.shininess;
    vec3 r = reflect(l, normalize(normal));
    vec3 v = normalize(fs_in.cameraPosition - fs_in.tangeantWorldPosition);
    float specFactor = pow(max(0, dot(r, v)), shininess);
    vec3 specularColor = specFactor * light.specular * color * material.specular;

    return diffuseColor + ambientColor + specularColor;
}


void main() {
    vec2 textureCoordinates = fs_in.textureCoordinates * 2 + 0.5;
//    textureCoordinates.y *= 2;

    textureCoordinates = getParallaxMappingCoordinates(
        textureCoordinates,
        normalize(fs_in.cameraPosition - fs_in.tangeantWorldPosition)
    );

//    if(textureCoordinates.x > 1.0 || textureCoordinates.y > 1.0 || textureCoordinates.x < 0.0 || textureCoordinates.y < 0.0)
//        discard;

    vec3 normal = texture(material.normalMap, textureCoordinates).rgb;
    normal = normalize(normal * 2.0 - 1.0);

	vec3 finalColor = vec3(0.1);

	if (directionalLight.enabled != 0)
	    finalColor += getDirectionalLightColor(directionalLight, fs_in.directionalLightDirection, normal, textureCoordinates);
	else
	    finalColor = texture(material.diffuseMap, textureCoordinates).xyz;

	for (int i = 0; i < amountOfPointLights; i++)
	    finalColor += getPointLightColor(pointLights[i], fs_in.pointLightsPositions[i], normal, textureCoordinates);

	FragColor = vec4(finalColor, 1);
}
