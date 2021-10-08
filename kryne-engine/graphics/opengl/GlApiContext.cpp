/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#include "GlApiContext.hpp"

#include <core/Window.hpp>
#include <core/Logger.hpp>
#include <graphics/opengl/GlTexture.hpp>

namespace KryneEngine
{
    eastl::shared_ptr<Texture> &&GlApiContext::CreateTexture(const KryneEngine::Texture::Description &_desc)
    {
        return eastl::move(eastl::shared_ptr<Texture>((Texture *)new GLTexture(_desc)));
    }

    void GlApiContext::SetMainThread()
    {
        glfwMakeContextCurrent(m_associatedWindow.GetUnderlyingGlfwWindow());
    }

    GlApiContext::GlApiContext(Window &_window, const GLContextParameters *_params)
        : GraphicsApiContext(_window)
        , m_contextParameters(_params != nullptr ? *_params : GLContextParameters())
    {
    }

    void GlApiContext::SetUpWindowHints()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_contextParameters.m_version.m_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_contextParameters.m_version.m_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, m_contextParameters.m_profile);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, m_contextParameters.m_useDebugLayer);
    }


    void GlApiContext::FinishContextSetup(GLFWwindow *_window)
    {
        Logger::Log(Logger::Level::Verbose, Logger::Category::Rendering, "OpenGL context setup finishing");

        Assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
               "Unable to load GLAD");

        if (m_contextParameters.m_useDebugLayer)
        {
            s32 flags;
            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

            if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
            {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(glDebugOutput, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            }
        }
    }
}