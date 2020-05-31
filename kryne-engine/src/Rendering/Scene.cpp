//
// Created by max on 22/04/19.
//

#include "kryne-engine/Rendering/Scene.h"

Scene::Scene(Window *window, Camera *camera)
{
    this->window = window;
    this->mainRenderer = new MainRenderer(camera, this->window->getWidth(), this->window->getHeight());
    this->shadowMapHandler = new ShadowMapHandler(camera);

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
    this->shadowMapHandler->renderShadowMaps(window, &rootNodes, directionalLight, &pointLights, params);

    params->insertLoopLongParameter("directionalLight", directionalLight);
    params->insertLoopLongParameter("pointLights", &pointLights);
    this->mainRenderer->render(window, &rootNodes, params);
}


Camera *Scene::getCamera() const {
    return mainRenderer->getCamera();
}


void Scene::setDirectionalLight(DirectionalLight *light)
{
    assert(light != nullptr);
    Scene::directionalLight = light;

    if (mainRenderer->getSkybox())
        mainRenderer->getSkybox()->setLightDirection(-directionalLight->getDirection());
}


void Scene::addPointLight(PointLight *light)
{
    assert(light != nullptr);
    pointLights.push_back(light);
}


void Scene::setSkybox(Skybox *skybox, SkyboxDrawOrder drawOrder)
{
    this->mainRenderer->setSkybox(skybox, drawOrder, directionalLight);
}

ShadowMapHandler *Scene::getShadowMapHandler() const {
    return shadowMapHandler;
}
