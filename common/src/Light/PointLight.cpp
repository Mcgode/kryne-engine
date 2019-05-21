//
// Created by max on 21/05/19.
//

#include "common/Light/PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 diffuseColor, glm::vec3 *ambientColor, glm::vec3 *specularColor)
{
    this->position = position;
    this->diffuseColor = diffuseColor;
    this->ambientColor = ambientColor ? *ambientColor : 0.1f * this->diffuseColor;
    this->specularColor = specularColor ? *specularColor : this->diffuseColor;

    this->constantAttenuation = 1;
    this->linearAttenuation = 1.0 / 20.0;
    this->quadraticAttenuation = 0.0;
}


void PointLight::shaderSet(Shader *shader, std::string uniformName)
{
    shader->setVec3(uniformName.append(".position"), position);

    shader->setVec3(uniformName.append(".ambient"), ambientColor);
    shader->setVec3(uniformName.append(".diffuse"), diffuseColor);
    shader->setVec3(uniformName.append(".specular"), specularColor);

    shader->setFloat(uniformName.append(".constant"), constantAttenuation);
    shader->setFloat(uniformName.append(".linear"), linearAttenuation);
    shader->setFloat(uniformName.append(".quadratic"), quadraticAttenuation);
}


void PointLight::shaderSetArray(Shader *shader, std::string uniformArrayName, size_t index)
{
    shaderSet(shader, uniformArrayName.append("[").append(std::to_string(index)).append("]"));
}


std::vector<PointLight*> PointLight::getClosestPointLights(std::vector<PointLight*> pointLightList,
                                                          glm::vec3 position,
                                                          size_t maximumAmount)
{
    std::vector<PointLight *> v;

    for (PointLight *light : pointLightList) {
        PointLight *l = light;
        float d = glm::distance(light->position, position);
        size_t i = 0;
        while (i < v.size() && i < maximumAmount) {
            if (glm::distance(v[i]->position, position) > d) {
                PointLight *tmp = v[i];
                v[i] = l;
                l = tmp;
            }
            i++;
        }

        if (v.size() < maximumAmount) v.push_back(l);
    }

    return v;
}


void PointLight::setAttenuation(float constant, float linear, float quadratic)
{
    constantAttenuation = constant;
    linearAttenuation = linear;
    quadraticAttenuation = quadratic;
}
