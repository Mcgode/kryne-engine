/**
 * @file
 * @author max
 * @date 10/2/21.
 */

#include "Window.hpp"

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
    }
}