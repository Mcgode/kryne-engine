//
// Created by max on 22/04/19.
//

#include "common/Scene.h"

Scene::Scene(Camera *camera, int window_width, int window_height)
{
    this->window = new Window(window_width, window_height);
    this->camera = camera;
    this->projection = glm::perspective(3.14 / 2.0, 16.0 / 9.0, 0.1, 150.0);

    glEnable(GL_DEPTH_TEST);
}


Scene::~Scene()
{
    delete window;
    delete camera;
    for (HierarchicalNode *node: rootNodes)
        delete node;
}


Camera *Scene::updateCamera(Camera *newCamera)
{
    auto oldCamera = this->camera;
    this->camera = newCamera;
    return oldCamera;
}


void Scene::addDrawable(HierarchicalNode *node)
{
    this->rootNodes.push_back(node);
}


void Scene::mainRenderLoop(AdditionalParams_t *params)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (HierarchicalNode *node: rootNodes)
        node->draw(this, this->camera->getViewMatrix(), glm::mat4(1.0f), params);
}

void Scene::drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model, AdditionalParams_t *params)
{
    obj->getShader()->use();

    if (obj->isSupportingLighting()) {
        // TODO : Support scene lighting handling
    }

    obj->draw(projection, view, model, params);

    Shader::resetUse();
}


void Scene::runLoop(AdditionalParams_t *params)
{
    this->window->runLoop(this, params);
}


Camera *Scene::getCamera() const {
    return camera;
}
