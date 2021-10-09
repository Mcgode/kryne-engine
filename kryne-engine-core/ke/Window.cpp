/**
 * @file
 * @author max
 * @date 10/2/21.
 */

#include "Window.hpp"

#if defined(GRAPHICS_API_GL)
    #include <ke/graphics/opengl/GlApiContext.hpp>
#elif defined(GRAPHICS_API_VK)
    #include <ke/graphics/vulkan/VkApiContext.hpp>
#endif

namespace KryneEngine
{

    glm::uvec2 Window::GetSize() const
    {
        return m_description.m_size;
    }

    Window::Window(Application &_application, const WindowDescription &_description, void *_additionalParameters)
        : m_application(_application)
        , m_description(_description)
    {
        Assert(!GraphicsApiContext::HasAssociatedContext(), "New graphical contexts and windows must be created in a blank thread");

#if defined(GRAPHICS_API_GL)
        m_graphicsContext.reset(new GlApiContext(*this, static_cast<GLContextParameters*>(_additionalParameters)));
#elif defined(GRAPHICS_API_VK)
        // TODO
#endif

        m_graphicsContext->SetUpWindowHints();
        m_glfwWindow = glfwCreateWindow((s32) m_description.m_size.x,
                                        (s32) m_description.m_size.y,
                                        m_description.m_name.c_str(),
                                        nullptr,
                                        nullptr);
        Assert(m_glfwWindow != nullptr, "Could not initialize window");

        GraphicsApiContext::ThreadSetContext(m_graphicsContext);
        m_graphicsContext->SetMainThread();

        m_graphicsContext->FinishContextSetup(m_glfwWindow);
    }
}