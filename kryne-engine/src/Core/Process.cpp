//
// Created by max on 16/10/2019.
//

#include "kryne-engine/Core/Process.h"

Process::Process(OldCamera *camera, uint16_t windowWidth, uint16_t windowHeight)
{
    this->processWindow = new Window(windowWidth, windowHeight);
    this->processWindow->setCurrentCamera(camera);

    this->scene = new RenderScene(this->processWindow, camera);
}


void Process::runProcess(AdditionalParameters *parameters)
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


Window *Process::getProcessWindow() const
{
    return processWindow;
}


RenderScene *Process::getScene() const
{
    return scene;
}
