/**
 * @file
 * @author Max Godefroy
 * @date 11/10/2019
 */

#ifndef INC_KRYNE_ENGINE_SHADOW_MAP_HANDLER_H
#define INC_KRYNE_ENGINE_SHADOW_MAP_HANDLER_H

#include <kryne-engine/Core/Window.h>
#include <kryne-engine/3DObjects/HierarchicalNode.h>
#include <kryne-engine/Light/LightingRegistry.h>
#include <map>
#include "DirectionalShadowMapRendering.h"

class ShadowMapHandler {

public:

    explicit ShadowMapHandler(OldCamera *mainCamera);

    void renderShadowMaps(Window *window, std::vector<HierarchicalNode *> *rootNodes,
                          const shared_ptr<LightingRegistry>& lightingRegistry,
                          AdditionalParameters *params);

    std::vector<GLuint> getDirectionalShadowMaps(OldDirectionalLight *directionalLight);

    std::vector<glm::mat4> getLightSpaceMatrices(OldDirectionalLight *directionalLight);

    void updateCamera(OldCamera *newCamera);

private:

    OldCamera *mainCamera;

    std::map<OldDirectionalLight *, DirectionalShadowMapRendering *> directionalShadowMaps;

};


#endif //INC_KRYNE_ENGINE_SHADOW_MAP_HANDLER_H
