//
// Created by max on 11/10/2019.
//

#include "common/Rendering/ShadowMapping/ShadowMapHandler.h"

void ShadowMapHandler::renderShadowMaps(Window *window, std::vector<HierarchicalNode *> *rootNodes,
                                        DirectionalLight *directionalLight, std::vector<PointLight *> *pointLights,
                                        AdditionalParameters *params)
{
    if (directionalLight->isCastingShadow())
    {
        if (directionalShadowMaps.find(directionalLight) == directionalShadowMaps.end())
            directionalShadowMaps.insert(std::pair<DirectionalLight *, DirectionalShadowMapRendering *>(
                    directionalLight, new DirectionalShadowMapRendering(directionalLight)));

        directionalShadowMaps.find(directionalLight)->second->render(window, rootNodes, params);
    }

    for (PointLight *pointLight: *pointLights) {
        if (pointLight->isCastingShadow()) {
            // TODO: Render shadow map
        }
    }
}


GLuint ShadowMapHandler::getDirectionalShadowMap(DirectionalLight *directionalLight)
{
    auto pair = directionalShadowMaps.find(directionalLight);
    if (pair == directionalShadowMaps.end())
        return 0;
    else
        return pair->second->getShadowMap();
}
