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

    GlApiContext::GlApiContext(Window &_window) : GraphicsApiContext(_window)
    {
    }
}