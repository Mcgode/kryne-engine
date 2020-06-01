//
// Created by max on 6/1/20.
//

#ifndef INC_3D_DEMOS_LIGHTINGREGISTRY_H
#define INC_3D_DEMOS_LIGHTINGREGISTRY_H

#include <memory>
#include "DirectionalLight.h"
#include "PointLight.h"

using namespace std;

class LightingRegistry {

public:
    vector<DirectionalLight *> * getDirectionalLights();

    vector<PointLight*> * getPointLights();

    void registerDirectionalLight(DirectionalLight *light);

    void registerPointLight(PointLight *light);

private:
    vector<DirectionalLight *> directionalLights {};

    vector<PointLight *> pointLights {};
};


#endif //INC_3D_DEMOS_LIGHTINGREGISTRY_H
