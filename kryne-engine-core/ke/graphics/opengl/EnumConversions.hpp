/**
 * @file
 * @author Max Godefroy
 * @date 04/08/2021.
 */

#pragma once

#include <ke/graphics/Texture.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace KryneEngine
{
    constexpr GLenum GetTextureType(Texture::Types _type)
    {
        switch (_type)
        {
            case Texture::Types::Texture2D:
                return GL_TEXTURE_2D;

            case Texture::Types::Texture2DArray:
                return GL_TEXTURE_2D_ARRAY;

            case Texture::Types::Texture3D:
                return GL_TEXTURE_3D;

            case Texture::Types::CubeMap:
                return GL_TEXTURE_CUBE_MAP;

            default:
                Assert(false, "Unknown/unsupported texture type");
                return 0;
        }
    }

    constexpr GLint GetTextureWrap(Texture::Wrap _wrap)
    {
        switch (_wrap)
        {
            case Texture::Wrap::ClampToEdge:
                return GL_CLAMP_TO_EDGE;

            case Texture::Wrap::Repeat:
                return GL_REPEAT;

            case Texture::Wrap::MirrorRepeat:
                return GL_MIRRORED_REPEAT;

            case Texture::Wrap::ClampToBorder:
                return GL_CLAMP_TO_BORDER;
        }
    }
}