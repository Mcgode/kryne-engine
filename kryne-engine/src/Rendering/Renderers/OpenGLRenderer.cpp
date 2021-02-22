/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#include "kryne-engine/Rendering/Renderers/OpenGLRenderer.h"


void OpenGLRenderer::handleMesh(IRenderMesh *renderMesh)
{
    const auto camera = this->mainCamera.lock();

    if (!camera) return;

    const auto& material = renderMesh->getMaterial();
    const auto& geometry = renderMesh->getGeometry();
    const auto transform = renderMesh->getEntity()->getTransform();

    material->prepareShader(geometry.get());

    // Only update external rendering state once, before drawing any object.
    // Since each object can have a different required state in this regard, it needs to be checked every single time.
    // No need to reset to a base state, since it will be updated dynamically, to fit the required state.

    if (context->renderingState->getSide() != material->getSide())
        context->renderingState->setSide(material->getSide());

    if (context->renderingState->isDepthTestEnabled() != material->isDepthTest())
        context->renderingState->setDepthTest(material->isDepthTest());

    if (context->renderingState->isDepthWriteEnabled() != material->isWriteDepth())
        context->renderingState->setDepthWrite(material->isWriteDepth());

    // Renderer-level uniforms
    material->setUniform("projectionMatrix", camera->getProjectionMatrix());
    material->setUniform("viewMatrix", camera->getViewMatrix());
    material->setUniform("cameraPosition", camera->getTransform()->getWorldPosition());
    material->setUniform("worldMatrix", transform->getWorldMatrix());
    material->setUniform("normalMatrix", transform->getNormalMatrix());

    // Run mesh updates
    renderMesh->onBeforeRender(camera.get());

    // Upload uniforms
    material->getShader()->updateUniforms();

    // Finally draw the object
    geometry->draw(material->getPrimitiveType());

    // Reset shader use, just in case
    material->resetUse();
}
