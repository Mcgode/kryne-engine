/**
 * @file
 * @author Max Godefroy
 * @date 17/04/2021.
 */

#include "kryne-engine/Rendering/Processes/ShadowMappingProcess.hpp"

vector<Camera *> ShadowMappingProcess::prepareFrame(LoopRenderer *renderer, Scene *scene)
{
    return vector<Camera *>();
}

void ShadowMappingProcess::render(LoopRenderer *renderer,
                                  const std::vector<RenderMesh *> &meshes,
                                  const std::unordered_map<Camera *, LoopRenderer::FrustumCullingData> &frustumCullingMap)
{

}
