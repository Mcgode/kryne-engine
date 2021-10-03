/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once
#include <graphics/core/GraphicsApiContext.hpp>
#include <graphics/opengl/GlContextParameters.hpp>

namespace KryneEngine
{
    class GlApiContext: public GraphicsApiContext
    {
    public:
        /**
         * @brief Initialize the OpenGL graphic context
         *
         * @param _window The window that spawned the context
         * @param _params The additional parameters for the OpenGL context
         */
        explicit GlApiContext(Window &_window, const GLContextParameters *_params);

    private:
        /// The additional parameters for the OpenGL context
        const GLContextParameters m_contextParameters;

    public:
        // Override
        void SetMainThread() override;

    protected:
        // Override
        void SetUpWindowHints() override;

    public:
        // Override
        eastl::shared_ptr<Texture> &&CreateTexture(const Texture::Description &_desc) override;
    };
}


