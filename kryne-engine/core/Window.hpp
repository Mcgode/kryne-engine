/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once


#include <glm/vec2.hpp>


namespace KryneEngine
{
    class Application;

    struct WindowDescription
    {
        bool hidden = false;
        glm::uvec2 size { 1920, 1080 };
    };

    class Window
    {
        friend class Application;

    private:
        explicit Window(Application& _application, const WindowDescription& _description);

        Application& m_application;

        /// The parameters of this window
        WindowDescription m_description;

    public:
        /// @brief Retrieve the window's size. Can be 0.
        [[nodiscard]] glm::uvec2 GetSize() const;
    };
}