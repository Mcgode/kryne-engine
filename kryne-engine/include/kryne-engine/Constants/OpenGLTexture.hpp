/**
 * @file
 * @author Max Godefroy
 * @date 08/04/2021.
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <kryne-engine/Textures/Texture.h>


namespace Constants {

    namespace {

        constexpr GLenum formatTable[] = {
            GL_RED,
            GL_RG,
            GL_RGB,
            GL_RGBA,
            GL_DEPTH_COMPONENT,
            GL_DEPTH_STENCIL
        };

        constexpr GLenum typesTables[] = {
            GL_UNSIGNED_BYTE,
            GL_BYTE,
            GL_HALF_FLOAT,
            GL_FLOAT,
        };

    }

    constexpr GLenum openGLFormat(Texture::Formats format)
    {
        return formatTable[static_cast<int>(format)];
    }

    constexpr GLenum openGLInternalFormat(Texture::Formats format, Texture::InternalSizes size)
    {
        switch (size)
        {
            case Texture::InternalSizes::Fit:
                return formatTable[static_cast<int>(format)];

            case Texture::InternalSizes::Byte:
                switch (format)
                {
                    case Texture::Formats::RED:
                        return GL_R8;

                    case Texture::Formats::RG:
                        return GL_RG8;

                    case Texture::Formats::RGB:
                        return GL_RGB8;

                    case Texture::Formats::RGBA:
                        return GL_RGBA8;

                    default:
                        return formatTable[static_cast<int>(format)];
                }

            case Texture::InternalSizes::HalfFloat:
                switch (format)
                {
                    case Texture::Formats::RED:
                        return GL_R16F;

                    case Texture::Formats::RG:
                        return GL_RG16F;

                    case Texture::Formats::RGB:
                        return GL_RGB16F;

                    case Texture::Formats::RGBA:
                        return GL_RGBA16F;

                    case Texture::Formats::DEPTH:
                        return GL_DEPTH_COMPONENT16;

                    default:
                        return formatTable[static_cast<int>(format)];
                }

            case Texture::InternalSizes::Float:
                switch (format)
                {
                    case Texture::Formats::RED:
                        return GL_R32F;

                    case Texture::Formats::RG:
                        return GL_RG32F;

                    case Texture::Formats::RGB:
                        return GL_RGB32F;

                    case Texture::Formats::RGBA:
                        return GL_RGBA32F;

                    default:
                        return formatTable[static_cast<int>(format)];
                }
        }

        return 0;
    }

    constexpr GLenum openGLDataType(Texture::DataTypes type)
    {
        return typesTables[static_cast<int>(type)];
    }

}