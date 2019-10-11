//
// Created by max on 21/05/19.
//

#include "common/Light/DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction,
                                   glm::vec3 diffuseColor,
                                   glm::vec3 *ambientColor,
                                   glm::vec3 *specularColor)
{
    this->direction = glm::normalize(direction);
    this->diffuseColor = diffuseColor;
    this->ambientColor = ambientColor ? *ambientColor : this->diffuseColor;
    this->specularColor = specularColor ? *specularColor : this->diffuseColor;
}


void DirectionalLight::shaderSet(Shader *shader, std::string uniformName)
{
    shader->setInt(uniformName + ".enabled", 1);

    shader->setVec3(uniformName + ".direction", direction);

    shader->setVec3(uniformName + ".ambient", ambientColor);
    shader->setVec3(uniformName + ".diffuse", diffuseColor);
    shader->setVec3(uniformName + ".specular", specularColor);
}


const glm::vec3 &DirectionalLight::getDirection() const
{
    return direction;
}


bool DirectionalLight::isCastingShadow() const
{
    return castsShadow;
}



