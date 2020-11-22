//
// Created by max on 11/10/2019.
//

#include <boost/shared_ptr.hpp>
#include "kryne-engine/Rendering/ShadowMapping/ShadowMapHandler.h"


ShadowMapHandler::ShadowMapHandler(OldCamera *mainCamera) : mainCamera(mainCamera) {}


void ShadowMapHandler::renderShadowMaps(Window *window, std::vector<HierarchicalNode *> *rootNodes,
                                        const shared_ptr<LightingRegistry>& lightingRegistry,
                                        AdditionalParameters *params)
{
    for (auto directionalLight: *lightingRegistry->getDirectionalLights()) {
        if (directionalLight->isCastingShadow())
        {
            if (directionalShadowMaps.find(directionalLight) == directionalShadowMaps.end())
                directionalShadowMaps.insert(std::pair<DirectionalLight *, DirectionalShadowMapRendering *>(
                        directionalLight, new DirectionalShadowMapRendering(directionalLight, this->mainCamera)));

            directionalShadowMaps.find(directionalLight)->second->render(window, rootNodes, params);
        }
    }

    for (auto pointLight: *lightingRegistry->getPointLights()) {
        if (pointLight->isCastingShadow()) {
            // TODO: Render shadow map
        }
    }
}


std::vector<GLuint> ShadowMapHandler::getDirectionalShadowMaps(DirectionalLight *directionalLight)
{
    auto pair = directionalShadowMaps.find(directionalLight);
    if (pair == directionalShadowMaps.end())
        return vector<GLuint> {};
    else
        return pair->second->getShadowMaps();
}

std::vector<glm::mat4> ShadowMapHandler::getLightSpaceMatrices(DirectionalLight *directionalLight)
{
    auto pair = directionalShadowMaps.find(directionalLight);
    if (pair == directionalShadowMaps.end())
        return std::vector<glm::mat4> {};
    else
        return pair->second->getLightSpaceMatrices();
}


void ShadowMapHandler::updateCamera(OldCamera *newCamera)
{
    this->mainCamera = newCamera;

    for (auto const& [_, renderer] : this->directionalShadowMaps) {
        renderer->updateCamera(newCamera);
    }
}
