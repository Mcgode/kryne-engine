/**
 * @file
 * @author Max Godefroy
 * @date 02/08/2021.
 */

#pragma once

#include <ke/graphics/Texture.hpp>
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
        void AcquireMemory(ImageData* _imageData) override;

        // Override
        void FreeMemory() override;

        /**
         * @brief Binds this texture as the current texture of its type
         */
        void Bind();

        /**
         * @brief Unbinds the texture of this type
         */
        void Unbind();

        /**
         * @brief Unbinds the texture of the provided type
         *
         * @param _type
         */
        static void Unbind(GLenum _type);

    protected:
        /// The OpenGL id for this texture. Is set to -1 if none is linked and thus the texture is invalid
        GLuint m_glId;

        const GLenum m_internalFormat;

        const GLenum m_format;

        const GLenum m_type;
    };
}

