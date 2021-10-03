/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once
#include <graphics/core/GraphicsApiContext.hpp>

namespace KryneEngine
{
    class GlApiContext: public GraphicsApiContext
    {
    public:
        /**
         * @brief Initialize the OpenGL graphic context
         *
         * @param _window The window that spawned the context
         */
        GlApiContext(Window& _window);

        /// @copydoc GraphicsApiContext::CreateTexture
        eastl::shared_ptr<Texture> &&CreateTexture(const Texture::Description &_desc) override;
    };
}


