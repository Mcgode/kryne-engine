/**
 * @file
 * @author max
 * @date 10/2/21.
 */

#include "Application.hpp"

#include <core/Window.hpp>
#include <GLFW/glfw3.h>


namespace KryneEngine
{
    Window& Application::CreateWindow(const KryneEngine::WindowDescription &_description)
    {
        auto& windowRef = m_windows.push_back();
        windowRef.reset(new Window(*this, _description));
        return *windowRef;
    }


    Application::Application()
    {
        glfwInit();
    }


    Application::~Application()
    {
        glfwTerminate();
    }
}
