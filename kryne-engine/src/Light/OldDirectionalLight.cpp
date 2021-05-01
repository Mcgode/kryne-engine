//
// Created by max on 21/05/19.
//

#include "kryne-engine/Light/OldDirectionalLight.h"

#include <utility>

OldDirectionalLight::OldDirectionalLight(glm::vec3 direction,
                                         glm::vec3 diffuseColor,
                                         glm::vec3 *ambientColor,
                                         glm::vec3 *specularColor)
{
    this->direction = glm::normalize(direction);
    this->diffuseColor = diffuseColor;
    this->ambientColor = ambientColor ? *ambientColor : this->diffuseColor;
    this->specularColor = specularColor ? *specularColor : this->diffuseColor;
}


void OldDirectionalLight::shaderSet(Shader *shader, std::string uniformName)
{
    shader->setUniform(uniformName + ".enabled", 1);

    shader->setUniform(uniformName + ".direction", direction);

    shader->setUniform(uniformName + ".ambient", ambientColor);
    shader->setUniform(uniformName + ".diffuse", diffuseColor);
    shader->setUniform(uniformName + ".specular", specularColor);
}


const glm::vec3 &OldDirectionalLight::getDirection() const
{
    return direction;
}


bool OldDirectionalLight::isCastingShadow() const
{
    return castsShadow;
}


uint16_t OldDirectionalLight::getShadowResolution() const
{
    return shadowResolution;
}


std::vector<double> OldDirectionalLight::getShadowCastRadii() const
{
    return shadowCastRadii;
}


void OldDirectionalLight::setCastsShadow(uint16_t resolution, double radius)
{
    this->castsShadow = true;
    this->shadowResolution = resolution;
    this->shadowCastRadii.push_back(radius);
}


void OldDirectionalLight::setCastsShadowCascaded(uint16_t resolution, std::vector<double> radii)
{
    this->castsShadow = true;
    this->shadowResolution = resolution;
    this->shadowCastRadii = std::move(radii);
}



