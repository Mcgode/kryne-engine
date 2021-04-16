/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#include <kryne-engine/Material/ShaderMaterial.hpp>
#include <kryne-engine/Geometry/BoxBufferGeometry.h>
#include <kryne-engine/Constants/CubeRenderMatrices.hpp>
#include "kryne-engine/Rendering/OpenGL/OpenGLRenderer.h"


OpenGLRenderer::OpenGLRenderer(GraphicContext *context, RenderingState *renderingState, const ivec2 &size) :
        LoopRenderer(context,
                     make_unique<OpenGLScreenFramebuffer>(size.x, size.y),
                     make_unique<OpenGLFramebuffer>(size.x, size.y),
                     make_unique<OpenGLFramebuffer>(size.x, size.y),
                     size),
        renderingState(renderingState)
{
    this->writeFramebuffer->addColorAttachment();
    this->writeFramebuffer->setUpDepthLayer();

    this->readFramebuffer->addColorAttachment();
    this->readFramebuffer->setUpDepthLayer();

    this->fullscreenPlane = make_unique<PlaneBufferGeometry>(2.f, 2.f);

    auto shader = make_unique<Shader>("Engine/Skybox");
    this->skyboxMaterial = make_shared<ShaderMaterial>(move(shader));
}


void OpenGLRenderer::defaultStateReset()
{
    this->renderingState->setScissor(false);
}


void OpenGLRenderer::handleMesh(RenderMesh *renderMesh)
{
    const auto camera = this->mainCamera;

    if (camera == nullptr) return;

    if (renderMesh->isFrustumCull())
    {
        const auto it = this->frustumCulled.find(camera);
        if (it == this->frustumCulled.end())
            return;

        const auto culledIt = it->second.culledMeshes.find(renderMesh);
        if (culledIt != it->second.culledMeshes.end() && culledIt->second)
            return;
    }

    const auto& material = renderMesh->getMaterial();
    const auto& geometry = renderMesh->getGeometry();
    const auto transform = renderMesh->getEntity()->getTransform();

    // Only update external rendering state once, before drawing any object.
    // Since each object can have a different required state in this regard, it needs to be checked every single time.
    // No need to reset to a base state, since it will be updated dynamically, to fit the required state.

    this->defaultStateReset();

    if (renderingState->getSide() != material->getSide())
        renderingState->setSide(material->getSide());

    if (renderingState->isDepthTestEnabled() != material->isDepthTest())
        renderingState->setDepthTest(material->isDepthTest());

    if (renderingState->isDepthWriteEnabled() != material->isWriteDepth())
        renderingState->setDepthWrite(material->isWriteDepth());

    // Renderer-level uniforms
    material->setUniform("projectionMatrix", camera->getProjectionMatrix());
    material->setUniform("viewMatrix", camera->getViewMatrix());
    material->setUniform("cameraPosition", camera->getTransform()->getWorldPosition());
    material->setUniform("modelMatrix", transform->getWorldMatrix());
    material->setUniform("normalMatrix", transform->getNormalMatrix());

    // Run mesh updates
    renderMesh->onBeforeRender(camera);

    // Upload uniforms
    material->prepareShader(geometry.get());

    // Finally draw the object
    geometry->draw(material->getPrimitiveType());

    // Reset shader use, just in case
    material->resetUse();
}


void OpenGLRenderer::prepareFrame()
{
    assertIsMainThread();

    LoopRenderer::prepareFrame();

    if (this->framePostProcessPasses.empty())
        this->screenFramebuffer->setAsRenderTarget();
    else
        this->writeFramebuffer->setAsRenderTarget();

    this->renderingState->setViewport(ivec2(0), this->rendererSize);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    renderingState->setDepthWrite(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


std::unordered_set<Entity *> OpenGLRenderer::parseScene(Scene *scene)
{
    unordered_set<Entity *> result;

    this->mainCamera->getProjectionData()->setViewportSize(this->rendererSize);
    result.emplace(this->mainCamera);

    return result;
}


void OpenGLRenderer::finishSceneRendering(Scene *scene)
{
    assertIsMainThread();

    const auto& envMap = scene->getSkyboxEnvMap();
    if (envMap != nullptr && **envMap != nullptr)
    {
        if (!envMap->isIblReady())
            this->pmremGenerator->processMap(envMap);

        this->defaultStateReset();

        this->skyboxMaterial->setUniform("skybox", **envMap);
        this->skyboxMaterial->setUniform("projectionMatrix", this->mainCamera->getProjectionMatrix());
        this->skyboxMaterial->setUniform("viewMatrix", mat4(mat3(this->mainCamera->getViewMatrix())));

        glDepthFunc(GL_LEQUAL);
        this->renderingState->setSide(BackSide);

        this->skyboxMaterial->prepareShader(this->cubeGeometry.get());
        this->cubeGeometry->draw(GL_TRIANGLES);

        glDepthFunc(GL_LESS);
    }
}


void OpenGLRenderer::handlePostProcessing()
{
    assertIsMainThread();

    this->pmremGenerator->runProcessing(this);

    for (size_t i = 0; i < this->framePostProcessPasses.size(); i++)
    {
        this->swapBuffers();
        auto &pass = this->framePostProcessPasses[i];
        pass->processPass(this,
                          this->readFramebuffer.get(),
                          (i + 1 == this->framePostProcessPasses.size()) ? this->screenFramebuffer.get() : this->writeFramebuffer.get());
    }
}


void OpenGLRenderer::textureRender(Material *material)
{
    glClearColor(0.f, 0.f, 0.f, 0.f);
    renderingState->setDepthWrite(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    this->defaultStateReset();

    // Only update external rendering state once, before drawing any object.
    // Since each object can have a different required state in this regard, it needs to be checked every single time.
    // No need to reset to a base state, since it will be updated dynamically, to fit the required state.

    renderingState->setSide(FrontSide);
    renderingState->setDepthTest(false);
    renderingState->setDepthWrite(false);

    // Upload uniforms
    material->prepareShader(this->fullscreenPlane.get());

    // Finally draw the object
    this->fullscreenPlane->draw(material->getPrimitiveType());

    // Reset shader use, just in case
    material->resetUse();
}


void OpenGLRenderer::renderCubeTexture(Framebuffer *framebuffer, Material *material, CubeTexture *cubeMap, int mipLevel)
{
    const auto vpp = this->renderingState->getViewportStart();
    const auto vps = this->renderingState->getViewportSize();

    this->renderingState->setViewport(ivec2(0), framebuffer->getSize());

    this->defaultStateReset();

    this->renderingState->setSide(BackSide);
    this->renderingState->setDepthTest(material->isDepthTest());
    this->renderingState->setDepthWrite(material->isWriteDepth());

    material->setUniform("projectionMatrix", Constants::cubeRenderProjectionMatrix);

    for (int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                               cubeMap->getId(), mipLevel);
        glClear(GL_COLOR_BUFFER_BIT);

        material->setUniform("viewMatrix", Constants::cubeRenderViewMatrices[i]);
        material->prepareShader(this->cubeGeometry.get());
        this->cubeGeometry->draw();
    }

    this->renderingState->setViewport(vpp, vps);
}
