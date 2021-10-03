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
    Window & Application::CreateWindow(const WindowDescription &_description, void *_additionalParams)
    {
        auto& windowRef = m_windows.push_back();
        windowRef.reset(new Window(*this, _description, nullptr));
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
