//
// Created by max on 22/04/19.
//

#include "common/Scene.h"

Scene::Scene(Camera *camera, int window_width, int window_height)
{
    this->window = new Window(window_width, window_height);
    this->camera = camera;
}


Scene::~Scene()
{
    delete window;
    delete camera;
    for (HierarchicalNode *node: rootNodes)
        delete node;
}


void Scene::updateCamera(Camera *newCamera)
{
    this->camera = newCamera;
}


void Scene::addDrawable(HierarchicalNode *node)
{
    this->rootNodes.push_back(node);
}


void Scene::mainRenderLoop(std::map<std::string, void *> *params)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (HierarchicalNode *node: rootNodes) {
        // TODO : Render nodes
    }
}
