//
// Created by max on 20/04/19.
//

#include <iostream>
#include <common/Scene.h>
#include <common/Camera/BasicCamera.h>
#include <common/GeometricShapes.h>

using namespace std;


int main(int argc, const char **argv)
{
    auto *pScene = new Scene(new BasicCamera(glm::vec3(0, 0, -2), glm::vec3(0, 0, 0)));

    vector<glm::vec3> vertices{};
    generateSphereShape(0.5, 10, 5, &vertices);

    auto *sh = new Shader("Quad/Color");
    auto *va = new VertexArray(&vertices);
    auto *obj = new BaseObject(sh, va);
    pScene->addDrawable(new HierarchicalNode(obj));

    sh->use();
    sh->setVec3("color", 1.0, 0.0, 0.0);
    Shader::resetUse();

    pScene->runLoop(nullptr);

    delete sh;
    delete va;
    delete pScene;

    return 0;
}