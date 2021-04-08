/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#include <kryne-engine/Rendering/OpenGL/OpenGLFramebuffer.h>
#include "kryne-engine/Core/GraphicContext/OpenGLContext.h"


void OpenGLContext::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    for (const auto context : OpenGLContext::runningContexts())
    {
        if (context->mainWindow == window)
        {
            context->updateSize(width, height);
            return;
        }
    }
}


void OpenGLContext::updateSize(int width, int height)
{
    this->windowSize = ivec2(width, height);
    this->renderer->updateRendererSize(this->windowSize);
}


void errorCallback(int error_code, const char error[])
{
    std::cerr << "GL Error (" << error_code << "): " << error << std::endl;
}


void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
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

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    this->mainWindow = glfwCreateWindow(baseWidth, baseHeight, "Kryne engine - OpenGL", nullptr, nullptr);
    if (this->mainWindow == nullptr)
    {
        std::cerr << "Could not initialize OpenGL window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Assert current context is main thread.
    assertIsMainThread();

    glfwMakeContextCurrent(this->mainWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(this->mainWindow, framebufferSizeCallback);
    glfwSetErrorCallback(errorCallback);

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    cout << "Initialized OpenGL context" << endl;

    this->input = PlayerInput::tryMakeInput(this->mainWindow);
    this->windowSize = ivec2(baseWidth, baseHeight);
    this->renderingState = make_unique<RenderingState>(this->windowSize, FrontSide, true);
    this->renderer = make_unique<OpenGLRenderer>(this, this->renderingState.get(), this->windowSize);

    OpenGLContext::runningContexts().emplace(this);
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
    assertIsMainThread();
    OpenGLContext::runningContexts().erase(this);
    glfwTerminate();
}


LoopRenderer *OpenGLContext::getRenderer()
{
    return reinterpret_cast<LoopRenderer *>(this->renderer.get());
}


unique_ptr<Framebuffer> OpenGLContext::makeFramebuffer(const ivec2 &size)
{
    return make_unique<OpenGLFramebuffer>(size.x, size.y);
}
