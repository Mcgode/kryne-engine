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
        for (auto i = 0; i < light->cascadedShadowMaps; i++)
        {
            if (light->shadowMapData[i] == nullptr)
            {
                auto camera = make_unique<Camera>(light->getProcess(), make_unique<OrthographicProjectionData>());
                camera->addComponent<DirectionalLightShadowCameraComponent>(i);
                auto framebuffer = renderer->getContext()->makeFramebuffer(ivec2(2048));
                framebuffer->setUpDepthLayer();
                light->shadowMapData[i] = make_unique<DirectionalLight::ShadowMapData>(move(camera), move(framebuffer));
            }

            auto &camera = light->shadowMapData[i]->shadowCamera;
            camera->getTransform()->unsafeSetParent(light->getTransform());
            camera->getComponent<DirectionalLightShadowCameraComponent>()->setMainCamera(mainCamera);
            camera->unsafeSetPreprocessingRequirement(mainCamera);

            cameras.push_back(camera.get());
        }
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
        for (auto i = 0; i < light->cascadedShadowMaps; i++)
        {
            if (light->shadowMapData[i] != nullptr)
            {
                const auto &shadowData = light->shadowMapData[i];
                renderer->setTargetFramebuffer(shadowData->shadowFramebuffer.get());
                renderer->clearBuffer(false, true, false);
                for (const auto &mesh: meshes)
                    renderer->renderMesh(mesh, shadowData->shadowCamera.get(), mesh->getMaterial()->getDepthMaterial());

                // Reset data
                shadowData->shadowCamera->getTransform()->unsafeSetParent(nullptr);
                shadowData->shadowCamera->unsafeSetPreprocessingRequirement(nullptr);
            }
        }
    }
}


ShadowMappingProcess::~ShadowMappingProcess()
{
    if (this->system)
        this->system->shadowProcess = nullptr;
}
