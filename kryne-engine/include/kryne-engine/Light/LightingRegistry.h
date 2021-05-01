/**
 * @file
 * @author Max Godefroy
 * @date 6/1/20
 */

#ifndef INC_KRYNE_ENGINE_LIGHTINGREGISTRY_H
#define INC_KRYNE_ENGINE_LIGHTINGREGISTRY_H

#include <memory>
#include "OldDirectionalLight.h"
#include "OldPointLight.h"

using namespace std;

class LightingRegistry {

public:
    vector<OldDirectionalLight *> * getDirectionalLights();

    vector<OldPointLight*> * getPointLights();

    void registerDirectionalLight(OldDirectionalLight *light);

    void registerPointLight(OldPointLight *light);

private:
    vector<OldDirectionalLight *> directionalLights {};

    vector<OldPointLight *> pointLights {};
};


#endif //INC_KRYNE_ENGINE_LIGHTINGREGISTRY_H
