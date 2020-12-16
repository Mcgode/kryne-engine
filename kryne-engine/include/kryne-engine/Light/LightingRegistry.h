/**
 * @file
 * @author Max Godefroy
 * @date 6/1/20
 */

#ifndef INC_KRYNE_ENGINE_LIGHTINGREGISTRY_H
#define INC_KRYNE_ENGINE_LIGHTINGREGISTRY_H

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


#endif //INC_KRYNE_ENGINE_LIGHTINGREGISTRY_H
