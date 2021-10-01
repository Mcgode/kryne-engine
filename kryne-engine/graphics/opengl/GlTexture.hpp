/**
 * @file
 * @author Max Godefroy
 * @date 02/08/2021.
 */

#pragma once

#include <graphics/core/Texture.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace KryneEngine
{
    class GLTexture: Texture
    {
    public:
        explicit GLTexture(const Texture::Description& _createDescription);

    private:
        // Override
        void UpdateDescription(const Description &_newDescription) override;

        // Override
        void AcquireMemory() override;

        // Override
        void FreeMemory() override;

    protected:

        /// The OpenGL id for this texture. Is set to -1 if none is linked and thus the texture is invalid
        GLuint m_glId;
    };
}

