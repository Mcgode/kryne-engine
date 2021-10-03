/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#include "GlApiContext.hpp"

#include <core/Window.hpp>
#include <graphics/opengl/GlTexture.hpp>

namespace KryneEngine
{
    eastl::shared_ptr<Texture> &&KryneEngine::GlApiContext::CreateTexture(const KryneEngine::Texture::Description &_desc)
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
    }
}