/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#include "GlApiContext.hpp"

#include <ke/Window.hpp>
#include <ke/Logger.hpp>
#include <ke/graphics/opengl/GlTexture.hpp>

constexpr const char* kLogCategory = "OpenGL context";

namespace KryneEngine
{
    void GlApiContext::LogOpenGLDebug(GLenum _source, GLenum _type, unsigned int _id, GLenum _severity, GLsizei _length,
                                      const char *_message, const void *_userParam)
    {
        // ignore non-significant error/warning codes
        if(_id == 131169 || _id == 131185 || _id == 131218 || _id == 131204) return;

        eastl::string message {}, category {};
        Logger::Level level;

        message.sprintf("(id: %i) %s", _id, _message);

        switch (_source)
        {
            case GL_DEBUG_SOURCE_API:             message += "\nSource: API)"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   message += "\nSource: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: message += "\nSource: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     message += "\nSource: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     message += "\nSource: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           message += "\nSource: Other"; break;
        }

        switch (_type)
        {
            case GL_DEBUG_TYPE_ERROR:               category = "Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: category = "Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  category = "Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         category = "Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         category = "Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              category = "Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          category = "Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           category = "Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               category = "Other"; break;
        }

        switch (_severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         level = Logger::Level::Error; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       level = Logger::Level::Warning; break;
            case GL_DEBUG_SEVERITY_LOW:          level = Logger::Level::Info; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: level = Logger::Level::Verbose; break;
        }

        Logger::Log(level, "OpenGL][" + category, message);
    }

    eastl::shared_ptr<Texture> &&GlApiContext::CreateTexture(const KryneEngine::Texture::Description &_desc)
    {
        return eastl::move(eastl::shared_ptr<Texture>((Texture *)new GLTexture(_desc)));
    }

    void GlApiContext::SetMainThread()
    {
        LOG_VERBOSE(kLogCategory, "Set current thread as main thread");

        glfwMakeContextCurrent(m_associatedWindow.GetUnderlyingGlfwWindow());
    }

    GlApiContext::GlApiContext(Window &_window, const GLContextParameters *_params)
        : GraphicsApiContext(_window)
        , m_contextParameters(_params != nullptr ? *_params : GLContextParameters())
    {
    }

    void GlApiContext::SetUpWindowHints()
    {
        LOG_VERBOSE(kLogCategory, "Setting up window hints");

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
        LOG_VERBOSE(kLogCategory, "Finishing setup");

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
                glDebugMessageCallback(LogOpenGLDebug, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            }
        }
    }
}