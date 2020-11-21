//
// Created by max on 22/04/19.
//

#include "kryne-engine/Rendering/RenderScene.h"

RenderScene::RenderScene(Window *window, Camera *camera)
{
    this->window = window;
    this->mainRenderer = new MainRenderer(camera, this->window->getWidth(), this->window->getHeight());
    this->shadowMapHandler = new ShadowMapHandler(camera);
    this->lightingRegistry = make_shared<LightingRegistry>();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
}


RenderScene::~RenderScene()
{
    delete mainRenderer;
    for (HierarchicalNode *node: rootNodes)
        delete node;
}


Camera *RenderScene::updateCamera(Camera *newCamera)
{

    return mainRenderer->updateCamera(newCamera);
}


void RenderScene::addDrawable(HierarchicalNode *node)
{
    this->rootNodes.push_back(node);
}


void RenderScene::renderLoop(AdditionalParameters *params)
{
    this->shadowMapHandler->renderShadowMaps(window, &rootNodes, lightingRegistry, params);

    params->insertLoopLongParameter("lightingRegistry", lightingRegistry.get());
    this->mainRenderer->render(window, &rootNodes, params);
}


Camera *RenderScene::getCamera() const {
    return mainRenderer->getCamera();
}


void RenderScene::setDirectionalLight(DirectionalLight *light)
{
    assert(light != nullptr);
    this->lightingRegistry->registerDirectionalLight(light);

    if (mainRenderer->getSkybox())
        mainRenderer->getSkybox()->setLightDirection(-light->getDirection());
}


void RenderScene::addPointLight(PointLight *light)
{
    assert(light != nullptr);
    lightingRegistry->registerPointLight(light);
}


void RenderScene::setSkybox(Skybox *skybox, SkyboxDrawOrder drawOrder)
{
    auto dirLights = *lightingRegistry->getDirectionalLights();
    this->mainRenderer->setSkybox(skybox, drawOrder, !dirLights.empty() ? dirLights[0] : nullptr);
}

ShadowMapHandler *RenderScene::getShadowMapHandler() const {
    return shadowMapHandler;
}
