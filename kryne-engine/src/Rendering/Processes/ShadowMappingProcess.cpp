/**
 * @file
 * @author Max Godefroy
 * @date 17/04/2021.
 */

#include "kryne-engine/Rendering/Processes/ShadowMappingProcess.hpp"

vector<Camera *> ShadowMappingProcess::prepareFrame(const LoopRenderer *renderer)
{
    swap(this->currentCDL, this->castingDirLights);
    this->castingDirLights.clear();

    // Main camera call should be safe.
    const auto mainCamera = renderer->getMainCamera();

    auto cameras = vector<Camera *>();
    for (const auto &light : this->currentCDL)
    {
        if (light->shadowMapData == nullptr)
        {
            auto camera = make_unique<Camera>(light->getProcess(), make_unique<OrthographicProjectionData>());
            camera->addComponent<DirectionalLightShadowCameraComponent>();
            auto framebuffer = renderer->getContext()->makeFramebuffer(ivec2(2048));
            light->shadowMapData = make_unique<DirectionalLight::ShadowMapData>(move(camera), move(framebuffer));
        }

        auto &camera = light->shadowMapData->shadowCamera;
        camera->getTransform()->unsafeSetParent(light->getTransform());
        camera->getComponent<DirectionalLightShadowCameraComponent>()->setMainCamera(mainCamera);
        camera->unsafeSetPreprocessingRequirement(mainCamera);

        cameras.push_back(camera.get());
    }

    return cameras;
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
