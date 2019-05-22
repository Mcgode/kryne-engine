//
// Created by max on 20/04/19.
//

#include <iostream>
#include <common/Scene.h>
#include <common/Camera/TrackballCamera.h>
#include <common/GeometricShapes.h>
#include "Sphere.h"

using namespace std;


int main(int argc, const char **argv)
{
    auto *pScene = new Scene(new TrackballCamera(glm::vec3(0, 0, 0)));

    auto *sh = new Shader("Sphere/Lighting");

    auto *dirLight = new DirectionalLight(glm::vec3(0, -1, -1));
    pScene->setDirectionalLight(dirLight);

    auto *pl = new PointLight(glm::vec3(-2, 0, 0), glm::vec3(0.3, 1, 0.3));
    pScene->addPointLight(pl);

    auto *obj = new Sphere(sh);
    pScene->addDrawable(new HierarchicalNode(obj));

    sh->use();
    sh->setVec3("color", 1.0, 0.0, 0.0);
    Shader::resetUse();

    pScene->runLoop();

    delete sh;
    delete dirLight;
    delete pScene;

    return 0;
}