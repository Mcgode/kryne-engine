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


void Scene::mainRenderLoop(AdditionalParameters *params)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 camPos = camera->getCurrentPosition();
    params->insertLoopLongParameter("cameraPosition", camPos);

    for (HierarchicalNode *node: rootNodes)
        node->draw(this, this->camera->getViewMatrix(), glm::mat4(1.0f), params);
}

void Scene::drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model, AdditionalParameters *params)
{
    obj->getShader()->use();

    if (obj->isSupportingLighting() && params->contains("cameraPosition")) {
        if (this->directionalLight) {
            directionalLight->shaderSet(obj->getShader(), "directionalLight");
            obj->getShader()->setVec3("cameraPosition", std::any_cast<glm::vec3>(params->get("cameraPosition")));
        } else
            obj->getShader()->setInt("directionalLight.enabled", 0);

        auto v = PointLight::getClosestPointLights(pointLights,
                                                   glm::vec3(glm::vec4(0, 0, 0, 1) * model),
                                                   obj->getMaximumSupportedPointLights());
        for (int i = 0; i < v.size(); i++)
            v[i]->shaderSetArray(obj->getShader(), "pointLights", i);
        obj->getShader()->setInt("amountOfPointLights", v.size());
    }

    obj->draw(projection, view, model, params);

    Shader::resetUse();
}


void Scene::runLoop(AdditionalParameters *params)
{
    this->window->runLoop(this, params);
}


Camera *Scene::getCamera() const {
    return camera;
}


void Scene::setDirectionalLight(DirectionalLight *light)
{
    assert(light != nullptr);
    Scene::directionalLight = light;
}


void Scene::addPointLight(PointLight *light)
{
    assert(light != nullptr);
    pointLights.push_back(light);
}
