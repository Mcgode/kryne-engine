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
        /**
         * @brief Logging function called for OpenGL debug output
         *
         * @param _source
         * @param _type
         * @param _id
         * @param _severity
         * @param _length
         * @param _message
         * @param _userParam
         */
        static void APIENTRY LogOpenGLDebug(GLenum _source,
                                            GLenum _type,
                                            unsigned int _id,
                                            GLenum _severity,
                                            GLsizei _length,
                                            const char *_message,
                                            const void *_userParam);

        /// The additional parameters for the OpenGL context
        const GLContextParameters m_contextParameters;

    public:
        // Override
        void SetMainThread() override;

    protected:
        // Override
        void SetUpWindowHints() override;

        // Override
        void FinishContextSetup(GLFWwindow *_window) override;

    public:
        // Override
        eastl::shared_ptr<Texture> &&CreateTexture(const Texture::Description &_desc) override;
    };
}


