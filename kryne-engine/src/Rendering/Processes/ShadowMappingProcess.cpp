/**
 * @file
 * @author Max Godefroy
 * @date 17/04/2021.
 */

#include "kryne-engine/Rendering/Processes/ShadowMappingProcess.hpp"

vector<Camera *> ShadowMappingProcess::prepareFrame(const LoopRenderer *renderer, Scene *scene)
{
    swap(this->currentCDL, this->castingDirLights);
    this->castingDirLights.clear();

    // Main camera call should be safe.
    const auto mainCamera = renderer->getMainCamera();

    for (const auto &light : this->currentCDL)
    {
        if (light->shadowMapData == nullptr)
        {
            // TODO : Instantiate ShadowMapData
            continue;
        }

        auto &camera = light->shadowMapData->shadowCamera;
        camera->getTransform()->unsafeSetParent(light->getTransform());
        camera->unsafeSetPreprocessingRequirement(mainCamera);
    }

    return vector<Camera *>();
}

void ShadowMappingProcess::render(LoopRenderer *renderer,
                                  const std::vector<RenderMesh *> &meshes,
                                  const std::unordered_map<Camera *, LoopRenderer::FrustumCullingData> &frustumCullingMap)
{
    // Reset light cameras unsafe pointers.
    // We do it at this point, because we're still sure that all the lights still exist.
    for (const auto &light : this->currentCDL)
    {
        if (light->shadowMapData)
        {
            light->shadowMapData->shadowCamera->getTransform()->unsafeSetParent(nullptr);
            light->shadowMapData->shadowCamera->unsafeSetPreprocessingRequirement(nullptr);
        }
    }
}


ShadowMappingProcess::~ShadowMappingProcess()
{
    if (this->system)
        this->system->shadowProcess = nullptr;
}
