//
// Created by max on 6/1/20.
//

#include "kryne-engine/Light/LightingRegistry.h"

vector<DirectionalLight *> * LightingRegistry::getDirectionalLights()
{
    return &this->directionalLights;
}

vector<PointLight *> * LightingRegistry::getPointLights()
{
    return &this->pointLights;
}


void LightingRegistry::registerDirectionalLight(DirectionalLight *light)
{
    this->directionalLights.push_back(light);
}


void LightingRegistry::registerPointLight(PointLight *light)
{
    this->pointLights.push_back(light);
}
