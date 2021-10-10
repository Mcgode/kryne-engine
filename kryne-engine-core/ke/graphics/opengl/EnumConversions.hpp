/**
 * @file
 * @author Max Godefroy
 * @date 04/08/2021.
 */

#pragma once

#include <ke/common/meta/Enums.hpp>
#include <ke/graphics/Texture.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace KryneEngine
{
    /**
     * @brief Retrieves the OpenGL texture type for the provided #Texture::Types value
     */
    ENUM_META_BEGIN(GLenum, Texture::Types, GetTextureType)

        ENUM_META(Texture::Types::Texture2D, GL_TEXTURE_2D)
        ENUM_META(Texture::Types::Texture2DArray, GL_TEXTURE_2D_ARRAY)
        ENUM_META(Texture::Types::Texture3D, GL_TEXTURE_3D)
        ENUM_META(Texture::Types::CubeMap, GL_TEXTURE_CUBE_MAP)
        default:
            Assert(false, "Unknown/unsupported texture type");
            return 0;

    ENUM_META_END()

    /**
     * @brief Retrieves the OpenGL wrapping enum for the provided #Texture::Wrap value.
     */
    ENUM_META_BEGIN(GLint, Texture::Wrap, GetTextureWrap)

        ENUM_META(Texture::Wrap::ClampToEdge, GL_CLAMP_TO_EDGE)
        ENUM_META(Texture::Wrap::Repeat, GL_REPEAT)
        ENUM_META(Texture::Wrap::MirrorRepeat, GL_MIRRORED_REPEAT)
        ENUM_META(Texture::Wrap::ClampToBorder, GL_CLAMP_TO_BORDER)

    ENUM_META_END()

    /**
     * @brief Retrieves the OpenGL texture internal format for the provided #Texture::Formats value
     */
    ENUM_META_BEGIN(GLenum, Texture::Formats, GetInternalFormat)

        ENUM_META(Texture::Formats::R8, GL_R8)
        ENUM_META(Texture::Formats::R16, GL_R16)
        ENUM_META(Texture::Formats::R16F, GL_R16F)
        ENUM_META(Texture::Formats::R32F, GL_R32F)
        
        ENUM_META(Texture::Formats::RG8, GL_RG8)
        ENUM_META(Texture::Formats::RG16, GL_RG16)
        ENUM_META(Texture::Formats::RG16F, GL_RG16F)
        ENUM_META(Texture::Formats::RG32F, GL_RG32F)
        
        ENUM_META(Texture::Formats::RGB8, GL_RGB8)
        ENUM_META(Texture::Formats::RGB16, GL_RGB16)
        ENUM_META(Texture::Formats::RGB16F, GL_RGB16F)
        ENUM_META(Texture::Formats::RGB32F, GL_RGB32F)
        
        ENUM_META(Texture::Formats::RGBA8, GL_RGBA8)
        ENUM_META(Texture::Formats::RGBA16, GL_RGBA16)
        ENUM_META(Texture::Formats::RGBA16F, GL_RGBA16F)
        ENUM_META(Texture::Formats::RGBA32F, GL_RGBA32F)

        ENUM_META(Texture::Formats::D16, GL_DEPTH_COMPONENT16)
        ENUM_META(Texture::Formats::D24, GL_DEPTH_COMPONENT24)
        ENUM_META(Texture::Formats::D32F, GL_DEPTH_COMPONENT32F)

        ENUM_META(Texture::Formats::D24_S8, GL_DEPTH24_STENCIL8)
        ENUM_META(Texture::Formats::D32F_S8, GL_DEPTH32F_STENCIL8)

        default:
            Assert(false, "Unknown/unsupported format type");
            return 0;

    ENUM_META_END()

    /**
     * @brief Retrieves the OpenGL texture high-level format for the provided #Texture::Formats value
     */
    ENUM_META_BEGIN(GLenum, Texture::Formats, GetFormat)

            ENUM_META(Texture::Formats::R8, GL_RED)
            ENUM_META(Texture::Formats::R16, GL_RED)
            ENUM_META(Texture::Formats::R16F, GL_RED)
            ENUM_META(Texture::Formats::R32F, GL_RED)

            ENUM_META(Texture::Formats::RG8, GL_RG)
            ENUM_META(Texture::Formats::RG16, GL_RG)
            ENUM_META(Texture::Formats::RG16F, GL_RG)
            ENUM_META(Texture::Formats::RG32F, GL_RG)

            ENUM_META(Texture::Formats::RGB8, GL_RGB)
            ENUM_META(Texture::Formats::RGB16, GL_RGB)
            ENUM_META(Texture::Formats::RGB16F, GL_RGB)
            ENUM_META(Texture::Formats::RGB32F, GL_RGB)

            ENUM_META(Texture::Formats::RGBA8, GL_RGBA)
            ENUM_META(Texture::Formats::RGBA16, GL_RGBA)
            ENUM_META(Texture::Formats::RGBA16F, GL_RGBA)
            ENUM_META(Texture::Formats::RGBA32F, GL_RGBA)

            ENUM_META(Texture::Formats::D16, GL_DEPTH_COMPONENT)
            ENUM_META(Texture::Formats::D24, GL_DEPTH_COMPONENT)
            ENUM_META(Texture::Formats::D32F, GL_DEPTH_COMPONENT)

            ENUM_META(Texture::Formats::D24_S8, GL_DEPTH_STENCIL)
            ENUM_META(Texture::Formats::D32F_S8, GL_DEPTH_STENCIL)

            default:
                Assert(false, "Unknown/unsupported format type");
                return 0;

    ENUM_META_END()

    /**
     * @brief Retrieves the OpenGL texture data type for the provided #Texture::Formats value
     */
    ENUM_META_BEGIN(GLenum, Texture::Formats, GetFormatType)

            ENUM_META(Texture::Formats::R8, GL_UNSIGNED_BYTE)
            ENUM_META(Texture::Formats::R16, GL_UNSIGNED_SHORT)
            ENUM_META(Texture::Formats::R16F, GL_HALF_FLOAT)
            ENUM_META(Texture::Formats::R32F, GL_FLOAT)

            ENUM_META(Texture::Formats::RG8, GL_UNSIGNED_BYTE)
            ENUM_META(Texture::Formats::RG16, GL_UNSIGNED_SHORT)
            ENUM_META(Texture::Formats::RG16F, GL_HALF_FLOAT)
            ENUM_META(Texture::Formats::RG32F, GL_FLOAT)

            ENUM_META(Texture::Formats::RGB8, GL_UNSIGNED_BYTE)
            ENUM_META(Texture::Formats::RGB16, GL_UNSIGNED_SHORT)
            ENUM_META(Texture::Formats::RGB16F, GL_HALF_FLOAT)
            ENUM_META(Texture::Formats::RGB32F, GL_FLOAT)

            ENUM_META(Texture::Formats::RGBA8, GL_UNSIGNED_BYTE)
            ENUM_META(Texture::Formats::RGBA16, GL_UNSIGNED_SHORT)
            ENUM_META(Texture::Formats::RGBA16F, GL_HALF_FLOAT)
            ENUM_META(Texture::Formats::RGBA32F, GL_FLOAT)

            ENUM_META(Texture::Formats::D16, GL_UNSIGNED_SHORT)
            ENUM_META(Texture::Formats::D24, GL_UNSIGNED_INT)
            ENUM_META(Texture::Formats::D32F, GL_FLOAT)

            ENUM_META(Texture::Formats::D24_S8, GL_UNSIGNED_INT_24_8)
            ENUM_META(Texture::Formats::D32F_S8, GL_FLOAT)

            default:
                Assert(false, "Unknown/unsupported format type");
                return 0;

    ENUM_META_END()
}