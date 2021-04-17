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

    return vector<Camera *>();
}

void ShadowMappingProcess::render(LoopRenderer *renderer,
                                  const std::vector<RenderMesh *> &meshes,
                                  const std::unordered_map<Camera *, LoopRenderer::FrustumCullingData> &frustumCullingMap)
{

}


ShadowMappingProcess::~ShadowMappingProcess()
{
    if (this->system)
        this->system->shadowProcess = nullptr;
}
