//
// Created by max on 20/04/19.
//

#include <iostream>
#include <kryne-engine/Rendering/Scene.h>
#include <kryne-engine/Camera/TrackballCamera.h>
#include <kryne-engine/Core/GeometricShapes.h>
#include <kryne-engine/Core/Process.h>
#include "SphereParallax.h"

using namespace std;


int main(int argc, const char **argv)
{
    auto process = new Process(new TrackballCamera(glm::vec3(0, 0, 0)));
    auto *pScene = process->getScene();

    auto *sh = new Shader("Sphere/Lighting");

    auto *dirLight = new DirectionalLight(glm::vec3(0, -1, -1), glm::vec3(0));
    pScene->setDirectionalLight(dirLight);

    auto *pl = new PointLight(glm::vec3(1.5, 1.8, 1.3), glm::vec3(1.0));
    pScene->addPointLight(pl);

    auto *obj = new SphereNormalMapping("bricks");
    auto node = new HierarchicalNode(obj);
    pScene->addDrawable(node);

    sh->use();
    sh->setVec3("color", 1.0, 0.0, 0.0);
    Shader::resetUse();

    process->runProcess();

    delete sh;
    delete dirLight;
    delete process;
    delete pl;
//    delete obj;

    return 0;
}