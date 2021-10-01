/**
 * @file
 * @author Max Godefroy
 * @date 02/08/2021.
 */

#include <graphics/opengl/EnumConversions.hpp>

#include "GlTexture.hpp"

namespace KryneEngine
{

    void GLTexture::UpdateDescription(const Texture::Description &_newDescription)
    {
        if (m_glId != 0)
        {
            // TODO: update texture data
        }

        m_description = _newDescription;
    }

    void GLTexture::AcquireMemory()
    {
        GLenum type = GetTextureType(m_description.m_type);
        Assert(type != 0, "No known types. Check the conversion function.");

#if !defined(KE_GL_LEGACY)
        glCreateTextures(type, 1, &m_glId);

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

#else
#error TODO: Not implemented yet
#endif
    }

    void GLTexture::FreeMemory()
    {

    }

    GLTexture::GLTexture(const Texture::Description &_createDescription)
        : Texture(_createDescription)
        , m_glId(0)
    {
    }
}
