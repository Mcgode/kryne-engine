/**
 * @file
 * @author max
 * @date 10/2/21.
 */

#pragma once

#include <EASTL/vector.h>
#include <EASTL/unique_ptr.h>

namespace KryneEngine
{
    struct WindowDescription;
    class Window;

    /**
     * @brief The central object for spawning windows and graphical contexts.
     *
     * @details
     * Application is accessed as a static singleton. You can spawn multiple graphics contexts, allowing you to do
     * multiple renders in parallel.
     */
    class Application
    {
    public:
        /**
         * @brief Meyers singleton of #Application
         */
        static Application& GetApplication()
        {
            static Application s_app;
            return s_app;
        }

    private:
        /// @brief Private constructor, for singleton purposes
        Application() = default;

    public:
        /**
         * @brief Instantiates a GLFW window and its associated graphical context
         *
         * @param _description The window
         *
         * @return A reference to the created window, which is owned exclusively by the application.
         */
        Window& CreateWindow(const WindowDescription& _description);

    private:
        /// @brief The windows of the application
        eastl::vector<eastl::unique_ptr<Window>> m_windows;
    };
}


