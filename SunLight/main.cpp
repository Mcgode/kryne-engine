//
// Created by max on 19/04/19.
//

#include <iostream>
#include <common/Scene.h>
#include <common/Camera/FirstPersonCamera.h>
#include <common/3DObjects/Model/ModelLoader.h>
#include "common/Shader.h"
#include "Floor.h"
#include "Model.h"
#include "StainedGlass.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void error_callback(int error_code, const char error[])
{
    std::cerr << "GL Error (" << error_code << "): " << error << std::endl;
}


int main(int argc, const char **argv)
{
    auto pScene = new Scene(new FirstPersonCamera(2.5f, 0.25f));

    auto *sh = new Shader("Sphere/Lighting");

    auto *dirLight = new DirectionalLight(glm::vec3(-1, -1, -0), glm::vec3(1));
    pScene->setDirectionalLight(dirLight);
//    auto data = ModelLoader::loadModel("TorusKnot.obj");

    auto obj = new Floor("concrete");
    auto transform = glm::translate(glm::mat4(1.0), glm::vec3(0, -2, 0));
    transform = glm::rotate(transform, -glm::pi<float>() / 2.f, glm::vec3(1, 0, 0));
    auto node = new HierarchicalNode(obj, transform);
    pScene->addDrawable(node);

    pScene->addDrawable(new HierarchicalNode(new Model("TorusKnot.obj", "concrete")));

    transform = glm::translate(glm::mat4(1), glm::vec3(3, 1, 0));
    transform = glm::rotate(transform, -glm::pi<float>() / 2.f, glm::vec3(0, 1, 0));
    pScene->addDrawable(new HierarchicalNode(new StainedGlass(), transform));

    sh->use();
    sh->setVec3("color", 1.0, 0.0, 0.0);
    Shader::resetUse();

    pScene->runLoop();

    delete sh;
    delete dirLight;
    delete pScene;

    return 0;
}