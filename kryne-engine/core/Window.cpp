/**
 * @file
 * @author max
 * @date 10/2/21.
 */

#include "Window.hpp"

#if defined(GRAPHICS_API_GL)
    #include <graphics/opengl/GlApiContext.hpp>
#elif defined(GRAPHICS_API_VK)
    #include <graphics/vulkan/VkApiContext.hpp>
#endif

namespace KryneEngine
{

    glm::uvec2 Window::GetSize() const
    {
        return m_description.size;
    }

    Window::Window(Application& _application, const WindowDescription &_description)
        : m_application(_application)
        , m_description(_description)
    {
#if defined(GRAPHICS_API_GL)
        m_graphicsContext.reset(new GlApiContext(*this));
#elif defined(GRAPHICS_API_VK)
        // TODO
#endif

        if (!GraphicsApiContext::HasAssociatedContext())
        {
            GraphicsApiContext::ThreadSetContext(m_graphicsContext);
            m_graphicsContext->SetMainThread();
        }
    }
}