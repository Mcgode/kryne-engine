//
// Created by max on 21/05/19.
//

#include "kryne-engine/Light/OldPointLight.h"

OldPointLight::OldPointLight(glm::vec3 position, glm::vec3 diffuseColor, glm::vec3 *ambientColor, glm::vec3 *specularColor)
{
    this->position = position;
    this->diffuseColor = diffuseColor;
    this->ambientColor = ambientColor ? *ambientColor : glm::vec3(0);
    this->specularColor = specularColor ? *specularColor : this->diffuseColor;

    this->constantAttenuation = 1;
    this->linearAttenuation = 1.0 / 20.0;
    this->quadraticAttenuation = 0.0;
}


void OldPointLight::shaderSet(Shader *shader, std::string uniformName)
{
    shader->setUniform(uniformName + ".position", position);

    shader->setUniform(uniformName + ".ambient", ambientColor);
    shader->setUniform(uniformName + ".diffuse", diffuseColor);
    shader->setUniform(uniformName + ".specular", specularColor);

    shader->setUniform(uniformName + ".constant", constantAttenuation);
    shader->setUniform(uniformName + ".linear", linearAttenuation);
    shader->setUniform(uniformName + ".quadratic", quadraticAttenuation);
}


void OldPointLight::shaderSetArray(Shader *shader, std::string uniformArrayName, size_t index)
{
    shaderSet(shader, uniformArrayName + "[" + std::to_string(index) + "]");
}


std::vector<OldPointLight*> OldPointLight::getClosestPointLights(const std::vector<OldPointLight*>& pointLightList,
                                                                 glm::vec3 position,
                                                                 size_t maximumAmount)
{
    std::vector<OldPointLight *> v;

    for (OldPointLight *light : pointLightList) {
        OldPointLight *l = light;
        float d = glm::distance(light->position, position);
        size_t i = 0;
        while (i < v.size() && i < maximumAmount) {
            if (glm::distance(v[i]->position, position) > d) {
                OldPointLight *tmp = v[i];
                v[i] = l;
                l = tmp;
            }
            i++;
        }

        if (v.size() < maximumAmount) v.push_back(l);
    }

    return v;
}


void OldPointLight::setAttenuation(float constant, float linear, float quadratic)
{
    constantAttenuation = constant;
    linearAttenuation = linear;
    quadraticAttenuation = quadratic;
}


bool OldPointLight::isCastingShadow() const
{
    return castsShadow;
}
