/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#include "kryne-engine/Rendering/RendererSystem.h"


void RendererSystem::runSystem(Scene *scene, bool allowConcurrentChunks)
{
    for (const auto entity : scene->getEntities())
    {
        for (const auto renderMesh : entity->getComponents<RenderMesh>())
            this->renderObject(renderMesh);
    }
}
