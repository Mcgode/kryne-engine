//
// Created by max on 07/10/2019.
//

#include "kryne-engine/Rendering/MainRenderer.h"


MainRenderer::MainRenderer(Camera *camera, unsigned int width, unsigned int height)
{
    this->camera = camera;
    this->projection = glm::perspective(3.14 / 2.0, (double) width / (double) height, 0.1, 150.0);
}


void
MainRenderer::renderMain(Window *window, std::vector<HierarchicalNode *> *rootNodes,
                         DirectionalLight *directionalLight, std::vector<PointLight *> *pointLights,
                         AdditionalParameters *params)
{
    glViewport(0, 0, window->getWidth(), window->getHeight());

    camera->frameUpdate(window);

    this->directionalLight = directionalLight;
    this->pointLights = pointLights;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 camPos = camera->getCurrentPosition();
    params->insertLoopLongParameter("cameraPosition", camPos);

    if (skyboxDrawOrder == SKYBOX_DRAW_FIRST)
        skybox->draw(projection, this->camera->getViewMatrix());

    for (HierarchicalNode *node: *rootNodes)
        node->draw(this, this->camera->getViewMatrix(), glm::mat4(1.0f), params);

    if (skyboxDrawOrder == SKYBOX_DRAW_LAST)
        skybox->draw(projection, this->camera->getViewMatrix());
}


void MainRenderer::drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model, AdditionalParameters *params)
{
    obj->getShader()->use();

    if (obj->isSupportingLighting() && params->contains("cameraPosition")) {
        if (this->directionalLight) {
            directionalLight->shaderSet(obj->getShader(), "directionalLight");
            obj->getShader()->setVec3("cameraPosition", std::any_cast<glm::vec3>(params->get("cameraPosition")));
        } else
            obj->getShader()->setInt("directionalLight.enabled", 0);

        auto v = PointLight::getClosestPointLights(*pointLights,
                                                   glm::vec3(glm::vec4(0, 0, 0, 1) * model),
                                                   obj->getMaximumSupportedPointLights());
        for (int i = 0; i < v.size(); i++)
            v[i]->shaderSetArray(obj->getShader(), "pointLights", i);
        obj->getShader()->setInt("amountOfPointLights", v.size());
    }

    obj->draw(projection, view, model, params);

    Shader::resetUse();
}


Camera *MainRenderer::getCamera() const
{
    return camera;
}


void MainRenderer::setSkybox(Skybox *skybox, SkyboxDrawOrder drawOrder, DirectionalLight *directionalLight)
{
    this->skybox = skybox;
    this->skyboxDrawOrder = drawOrder;

    if (skybox && directionalLight)
        skybox->setLightDirection(-directionalLight->getDirection());

    if (drawOrder != SKYBOX_NO_DRAW && !skybox) {
        std::cerr << "No skybox provided" << std::endl;
        exit(EXIT_FAILURE);
    }
}


Skybox *MainRenderer::getSkybox() const
{
    return skybox;
}

Camera *MainRenderer::updateCamera(Camera *newCamera)
{
    auto oldCamera = this->camera;
    this->camera = newCamera;
    return oldCamera;
}


MainRenderer::~MainRenderer()
{
    delete camera;
    delete skybox;
}


void MainRenderer::render(Window *window, std::vector<HierarchicalNode *> *rootNodes, AdditionalParameters *params)
{
    LightingRegistry *lightingRegistry = nullptr;
    if (params->contains("lightingRegistry")) {
        try {
            lightingRegistry = std::any_cast<LightingRegistry *>(params->get("lightingRegistry"));
        } catch (std::bad_any_cast& e) {}
    }


    if (lightingRegistry) {
        auto drawDirectionalLight = (*lightingRegistry->getDirectionalLights())[0];
        auto drawPointLights = lightingRegistry->getPointLights();
        renderMain(window, rootNodes, drawDirectionalLight, drawPointLights, params);
    } else {
        std::cerr << "Could not retrieve lights data" << std::endl;
        exit(EXIT_FAILURE);
    }
}
