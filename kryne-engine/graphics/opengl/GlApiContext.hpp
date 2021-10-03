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
        explicit GlApiContext(Window& _window);

        // Override
        void SetMainThread() override;

        // Override
        eastl::shared_ptr<Texture> &&CreateTexture(const Texture::Description &_desc) override;
    };
}


