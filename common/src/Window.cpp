//
// Created by max on 20/04/19.
//

#include <common/Scene.h>
#include "common/Window.h"


Window *current_window;


void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void errorCallback(int error_code, const char error[])
{
    std::cerr << "GL Error (" << error_code << "): " << error << std::endl;
}


Window::Window(uint32_t width, uint32_t height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    this->width = width;
    this->height = height;

    window = glfwCreateWindow(this->width, this->height, "GL window", nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "Could not initialize window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetErrorCallback(errorCallback);

    current_window = this;

    glfwSetKeyCallback(this->window, Window::keyCallback);
    glfwSetScrollCallback(this->window, Window::scrollCallback);
    glfwSetCursorPosCallback(this->window, Window::mouseMovementCallback);
}


void Window::keyCallback(GLFWwindow *_window, int key, int _scan_code, int action, int _mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(current_window->window, 1);
    else if (action == GLFW_PRESS){
        current_window->keysPressed.insert(key);
        current_window->currentScene->getCamera()->onKeyInput(current_window, key);
    } else if (action == GLFW_RELEASE) {
        current_window->keysPressed.erase(key);
    }
}


Window::~Window()
{
    glfwTerminate();
}


int Window::getWidth() const
{
    return width;
}


int Window::getHeight() const
{
    return height;
}


void Window::runLoop(Scene *scene, AdditionalParameters *params)
{
    assert(params != nullptr);

    currentScene = scene;

    while (!glfwWindowShouldClose(this->window)) {
        scene->mainRenderLoop(params);
        glfwSwapBuffers(this->window);
        glfwPollEvents();
        params->cleanupLoopParameters();
    }
}


void Window::scrollCallback(GLFWwindow *_w, double _, double scroll)
{
    current_window->currentScene->getCamera()->onScrollInput(current_window, scroll);
}


void Window::mouseMovementCallback(GLFWwindow *_w, double xPos, double yPos)
{
    current_window->currentScene->getCamera()->onMouseMovementInput(current_window, xPos, yPos);
}


GLFWwindow *Window::getGlfwWindow() const
{
    return window;
}
