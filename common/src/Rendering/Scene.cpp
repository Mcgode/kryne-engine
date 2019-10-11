//
// Created by max on 22/04/19.
//

#include "common/Rendering/Scene.h"

Scene::Scene(Camera *camera, int window_width, int window_height)
{
    this->window = new Window(window_width, window_height);
    this->mainRenderer = new MainRenderer(camera, window_width, window_height);

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
    delete window;
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
    params->insertLoopLongParameter("directionalLight", directionalLight);
    params->insertLoopLongParameter("pointLights", &pointLights);
    mainRenderer->render(window, &rootNodes, params);
}


void Scene::runLoop(AdditionalParameters *params)
{
    AdditionalParameters *p = params == nullptr ? new AdditionalParameters() : params;
    this->window->runLoop(this, p);
    if (params == nullptr)
        delete(p);
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
