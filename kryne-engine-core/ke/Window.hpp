/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once

#include <glm/vec2.hpp>
#include <EASTL/shared_ptr.h>
#include <EASTL/string.h>

#if defined(GRAPHICS_API_GL)
    #include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>


namespace KryneEngine
{
    class Application;
    class GraphicsApiContext;

    struct WindowDescription
    {
        bool m_hidden = false;

        glm::uvec2 m_size {1920, 1080 };

        eastl::string m_name;
    };

    class Window
    {
        friend class Application;

    private:
        /**
         * @brief  A private constructor, only accessible from #Application
         * @param _application @copybrief m_application
         * @param _description @copybrief m_description
         */
        explicit Window(Application &_application, const WindowDescription &_description, void *_additionalParameters);

        /// The application owning this window
        Application& m_application;

        /// The parameters of this window
        WindowDescription m_description;

    public:
        /// @brief Retrieve the underlying `GLFWwindow` object
        [[nodiscard]] GLFWwindow* GetUnderlyingGlfwWindow() const
        {
            return m_glfwWindow;
        }

    private:
        /// The underlying GLFW window
        GLFWwindow* m_glfwWindow;

        /// The graphics context spawned by this window.
        eastl::shared_ptr<GraphicsApiContext> m_graphicsContext {};

    public:
        /// @brief Retrieve the window's size. Can be 0.
        [[nodiscard]] glm::uvec2 GetSize() const;
    };
}