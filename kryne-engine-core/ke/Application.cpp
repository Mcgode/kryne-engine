/**
 * @file
 * @author max
 * @date 10/2/21.
 */

#include <ke/Logger.hpp>
#include <ke/Window.hpp>

#include "Application.hpp"


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
        LOG_VERBOSE("Application", " === Initializing application === ");

        LOG_VERBOSE("Application", "Booting GLFW");
        glfwInit();
    }


    Application::~Application()
    {
        LOG_VERBOSE("Application", " === Shutting down application === ");

        LOG_VERBOSE("Application", "Closing all windows");
        for (auto& windowPtr : m_windows)
        {
            windowPtr.reset();
        }

        LOG_VERBOSE("Application", "Shutting down GLFW");
        glfwTerminate();
    }
}
