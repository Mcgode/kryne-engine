//
// Created by max on 6/1/20.
//

#include "kryne-engine/Light/LightingRegistry.h"

vector<OldDirectionalLight *> * LightingRegistry::getDirectionalLights()
{
    return &this->directionalLights;
}

vector<OldPointLight *> * LightingRegistry::getPointLights()
{
    return &this->pointLights;
}


void LightingRegistry::registerDirectionalLight(OldDirectionalLight *light)
{
    this->directionalLights.push_back(light);
}


void LightingRegistry::registerPointLight(OldPointLight *light)
{
    this->pointLights.push_back(light);
}
