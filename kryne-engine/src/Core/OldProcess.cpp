//
// Created by max on 16/10/2019.
//

#include "kryne-engine/Core/OldProcess.h"

OldProcess::OldProcess(OldCamera *camera, uint16_t windowWidth, uint16_t windowHeight)
{
    this->processWindow = new Window(windowWidth, windowHeight);
    this->processWindow->setCurrentCamera(camera);

    this->scene = new RenderScene(this->processWindow, camera);
}


void OldProcess::runProcess(AdditionalParameters *parameters)
{
    AdditionalParameters *p = (parameters == nullptr) ? new AdditionalParameters() : parameters;

    while (!glfwWindowShouldClose(this->processWindow->getGlfwWindow())) {
        scene->renderLoop(p);
        glfwSwapBuffers(this->processWindow->getGlfwWindow());
        glfwPollEvents();
        p->cleanupLoopParameters();
    }

    if (parameters == nullptr)
        delete(p);
}


Window *OldProcess::getProcessWindow() const
{
    return processWindow;
}


RenderScene *OldProcess::getScene() const
{
    return scene;
}
