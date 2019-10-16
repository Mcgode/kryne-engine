//
// Created by max on 20/04/19.
//

#include <iostream>
#include <kryne-engine/Rendering/Scene.h>
#include <kryne-engine/Camera/TrackballCamera.h>
#include <kryne-engine/GeometricShapes.h>
#include "SphereParallax.h"

using namespace std;


int main(int argc, const char **argv)
{
    auto *pScene = new Scene(new TrackballCamera(glm::vec3(0, 0, 0)));

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

    pScene->runLoop();

    delete sh;
    delete dirLight;
    delete pScene;
    delete pl;
//    delete obj;

    return 0;
}