/**
 * @file
 * @author Max Godefroy
 * @date 02/08/2021.
 */

#include <ke/graphics/opengl/EnumConversions.hpp>
#include <ke/Logger.hpp>

#include "GlTexture.hpp"

namespace KryneEngine
{
    namespace
    {
        void SetTexture2DData(const Texture::Description& _desc, GLenum _internalFormat, GLenum _format, GLenum _type,
                              Texture::ImageData* _imageData)
        {
            const bool hasData = _imageData != nullptr && _imageData->size() != 0;
            LOG_IF_WARNING(hasData && _imageData->size() > 1, "OpenGL Texture", "Passing too much data (more image layers than required)");

            const u8 mipDataCount = hasData ? (*_imageData)[0].size() : 0;
            LOG_IF_WARNING(_desc.m_mipCount < mipDataCount, "OpenGL Texture", "Passing too much data (more mips than required)");

            for (u8 i = 0; i < mipDataCount || i == 0; i++)
            {
                void* imageData = hasData ? (*_imageData)[0][i] : nullptr;
                glTexImage2D(GL_TEXTURE_2D, i, _internalFormat, _desc.m_size.x, _desc.m_size.y, 0, _format, _type, imageData);
            }
        }
    }

    void GLTexture::UpdateDescription(const Texture::Description &_newDescription)
    {
        if (m_glId != 0)
        {
            // TODO: update texture data
        }

        m_description = _newDescription;
    }

    void GLTexture::AcquireMemory(ImageData* _imageData)
    {
        GLenum type = GetTextureType(m_description.m_type);
        Assert(type != 0, "No known types. Check the conversion function.");

        glCreateTextures(type, 1, &m_glId);
        Bind();

        switch (m_description.m_type)
        {
            case Texture::Types::Texture2D:
                SetTexture2DData(m_description, m_internalFormat, m_format, m_type, _imageData);
                break;
            default:
                Assert(false, "Not yet implemented");
                return;
        }

        glTextureParameteri(m_glId, GL_TEXTURE_WRAP_S, GetTextureWrap(m_description.m_wrapping.s));
        glTextureParameteri(m_glId, GL_TEXTURE_WRAP_T, GetTextureWrap(m_description.m_wrapping.t));
        glTextureParameteri(m_glId, GL_TEXTURE_WRAP_R, GetTextureWrap(m_description.m_wrapping.p));

        if (m_description.m_mipCount > 0)
        {
            bool linearMip = m_description.m_filtering.m_linearMips;
            glTextureParameteri(m_glId, GL_TEXTURE_MIN_FILTER, m_description.m_filtering.m_linearMinify
                                                               ? (linearMip ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST)
                                                               : (linearMip ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
            glTextureParameteri(m_glId, GL_TEXTURE_MAG_FILTER, m_description.m_filtering.m_linearMagnify
                                                               ? (linearMip ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST)
                                                               : (linearMip ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
        }
        else
        {
            glTextureParameteri(m_glId, GL_TEXTURE_MIN_FILTER, m_description.m_filtering.m_linearMinify ? GL_LINEAR : GL_NEAREST);
            glTextureParameteri(m_glId, GL_TEXTURE_MAG_FILTER, m_description.m_filtering.m_linearMagnify ? GL_LINEAR : GL_NEAREST);
        }
    }

    void GLTexture::FreeMemory()
    {
        glDeleteTextures(1, &m_glId);
        m_glId = 0;
    }

    GLTexture::GLTexture(const Texture::Description &_createDescription)
        : Texture(_createDescription)
        , m_glId(0)
        , m_internalFormat(GetInternalFormat(m_description.m_format))
        , m_format(GetFormat(m_description.m_format))
        , m_type(GetFormatType(m_description.m_format))
    {
    }

    void GLTexture::Bind()
    {
        if (m_glId == 0)
        {
            LOG_INFO("OpenGL texture", "Binding unacquired texture (gl id is zero)");
        }
        glBindTexture(GetTextureType(m_description.m_type), m_glId);
    }

    void GLTexture::Unbind()
    {
        glBindTexture(GetTextureType(m_description.m_type), 0);
    }

    void GLTexture::Unbind(GLenum _type)
    {
        glBindTexture(_type, 0);
    }
}
