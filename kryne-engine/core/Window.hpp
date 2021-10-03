/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once

#include <glm/vec2.hpp>
#include <EASTL/shared_ptr.h>


namespace KryneEngine
{
    class Application;
    class GraphicsApiContext;

    struct WindowDescription
    {
        bool hidden = false;
        glm::uvec2 size { 1920, 1080 };
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
        explicit Window(Application& _application, const WindowDescription& _description);

        /// The application owning this window
        Application& m_application;

        /// The parameters of this window
        WindowDescription m_description;

    private:
        /// The graphics context spawned by this window.
        eastl::shared_ptr<GraphicsApiContext> m_graphicsContext {};

    public:
        /// @brief Retrieve the window's size. Can be 0.
        [[nodiscard]] glm::uvec2 GetSize() const;
    };
}