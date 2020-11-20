//
// Created by max on 22/04/19.
//

#include "kryne-engine/Rendering/Scene.h"

Scene::Scene(Window *window, Camera *camera)
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


Scene::~Scene()
{
    delete mainRenderer;
    for (HierarchicalNode *node: rootNodes)
        delete node;
}


Camera *Scene::updateCamera(Camera *newCamera)
{

    return mainRenderer->updateCamera(newCamera);
}


void Scene::addDrawable(HierarchicalNode *node)
{
    this->rootNodes.push_back(node);
}


void Scene::renderLoop(AdditionalParameters *params)
{
    this->shadowMapHandler->renderShadowMaps(window, &rootNodes, lightingRegistry, params);

    params->insertLoopLongParameter("lightingRegistry", lightingRegistry.get());
    this->mainRenderer->render(window, &rootNodes, params);
}


Camera *Scene::getCamera() const {
    return mainRenderer->getCamera();
}


void Scene::setDirectionalLight(DirectionalLight *light)
{
    assert(light != nullptr);
    this->lightingRegistry->registerDirectionalLight(light);

    if (mainRenderer->getSkybox())
        mainRenderer->getSkybox()->setLightDirection(-light->getDirection());
}


void Scene::addPointLight(PointLight *light)
{
    assert(light != nullptr);
    lightingRegistry->registerPointLight(light);
}


void Scene::setSkybox(Skybox *skybox, SkyboxDrawOrder drawOrder)
{
    auto dirLights = *lightingRegistry->getDirectionalLights();
    this->mainRenderer->setSkybox(skybox, drawOrder, !dirLights.empty() ? dirLights[0] : nullptr);
}

ShadowMapHandler *Scene::getShadowMapHandler() const {
    return shadowMapHandler;
}
