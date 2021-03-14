/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#include "kryne-engine/Rendering/LoopRenderer.h"


LoopRenderer::FrustumCullingData::FrustumCullingData(Camera *camera) :
    frustum(camera->getProjectionMatrix() * camera->getViewMatrix())
{}


void LoopRenderer::prepareFrame()
{
    this->frustumCulled.clear();

    FrustumCullingData mainCamFCD(mainCamera);
    this->frustumCulled.emplace(mainCamera, mainCamFCD);
}


void LoopRenderer::computeFrustumCulling(RenderMesh *mesh)
{
    if (!mesh->isFrustumCull())
        return;

    for (auto &pair : this->frustumCulled)
    {
        pair.second.culledMeshes.emplace(mesh, !pair.second.frustum.sphereIntersects(mesh->getBoundingSphere()));
    }
}
