/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#include "OpenGLContext.h"


void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void errorCallback(int error_code, const char error[])
{
    std::cerr << "GL Error (" << error_code << "): " << error << std::endl;
}


OpenGLContext::OpenGLContext(GLuint baseWidth, GLuint baseHeight, GLint majorVersion, GLint minorVersion, GLint profile)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    this->mainWindow = glfwCreateWindow(baseWidth, baseHeight, "Kryne engine - OpenGL", nullptr, nullptr);
    if (this->mainWindow == nullptr)
    {
        std::cerr << "Could not initialize OpenGL window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(this->mainWindow);

    glfwSetFramebufferSizeCallback(this->mainWindow, framebufferSizeCallback);
    glfwSetErrorCallback(errorCallback);

    this->input = PlayerInput::tryMakeInput(this->mainWindow);
}


bool OpenGLContext::shouldStop()
{
    return glfwWindowShouldClose(this->mainWindow);
}


void OpenGLContext::stop()
{
    glfwSetWindowShouldClose(this->mainWindow, 1);
}


void OpenGLContext::endFrame()
{
    glfwSwapBuffers(this->mainWindow);
    input->willPollEvents();
    glfwPollEvents();
}


PlayerInput *OpenGLContext::getPlayerInput()
{
    return this->input.get();
}


OpenGLContext::~OpenGLContext()
{
    glfwTerminate();
}
