//
// Created by max on 11/10/2019.
//

#ifndef INC_3D_DEMOS_SHADOW_MAP_HANDLER_H
#define INC_3D_DEMOS_SHADOW_MAP_HANDLER_H

#include <kryne-engine/Core/Window.h>
#include <kryne-engine/3DObjects/HierarchicalNode.h>
#include <kryne-engine/Light/DirectionalLight.h>
#include <kryne-engine/Light/PointLight.h>
#include <map>
#include "DirectionalShadowMapRendering.h"

class ShadowMapHandler {

public:

    ShadowMapHandler(Camera *mainCamera);

    void renderShadowMaps(Window *window, std::vector<HierarchicalNode *> *rootNodes,
                          DirectionalLight *directionalLight, std::vector<PointLight *> *pointLights,
                          AdditionalParameters *params);

    std::vector<GLuint> getDirectionalShadowMaps(DirectionalLight *directionalLight);

    std::vector<glm::mat4> getLightSpaceMatrices(DirectionalLight *directionalLight);

    void updateCamera(Camera *newCamera);

private:

    Camera *mainCamera;

    std::map<DirectionalLight *, DirectionalShadowMapRendering *> directionalShadowMaps;

};


#endif //INC_3D_DEMOS_SHADOW_MAP_HANDLER_H
