//
// Created by max on 20/04/19.
//

#include "common/Window.h"

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
