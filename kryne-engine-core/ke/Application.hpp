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
     * This class will initialize GLFW, and will terminate it upon deletion.
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
        Application();

        /// @brief Private destructor.
        ~Application();

    public:
        /**
         * @brief Instantiates a GLFW window and its associated graphical context
         *
         * @param _description The window's description
         * @param _additionalParams Api-related additional parameters
         *
         * @return A reference to the created window, which is owned exclusively by the application.
         */
        Window &CreateWindow(const WindowDescription &_description, void* _additionalParams);

    private:
        /// @brief The windows of the application
        eastl::vector<eastl::unique_ptr<Window>> m_windows;
    };
}


