//
// Created by max on 21/05/19.
//

#include "kryne-engine/Light/DirectionalLight.h"

#include <utility>

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


uint16_t DirectionalLight::getShadowResolution() const
{
    return shadowResolution;
}


std::vector<double> DirectionalLight::getShadowCastRadii() const
{
    return shadowCastRadii;
}


void DirectionalLight::setCastsShadow(uint16_t resolution, double radius)
{
    this->castsShadow = true;
    this->shadowResolution = resolution;
    this->shadowCastRadii.push_back(radius);
}


void DirectionalLight::setCastsShadowCascaded(uint16_t resolution, std::vector<double> radii)
{
    this->castsShadow = true;
    this->shadowResolution = resolution;
    this->shadowCastRadii = std::move(radii);
}



