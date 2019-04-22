//
// Created by max on 20/04/19.
//

#include <iostream>
#include <common/Scene.h>
#include <common/BasicCamera.h>

using namespace std;


int main(int argc, const char **argv)
{
    auto *pScene = new Scene(new BasicCamera(glm::vec3(0, 0, -2), glm::vec3(0, 0, 0)));

    pScene->runLoop(nullptr);

    delete(pScene);

    return 0;
}