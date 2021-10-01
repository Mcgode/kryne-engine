/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#include "GlApiContext.hpp"
#include <graphics/opengl/GlTexture.hpp>

namespace KryneEngine
{
    eastl::shared_ptr<Texture> &&KryneEngine::GlApiContext::CreateTexture(const KryneEngine::Texture::Description &_desc)
    {
        return eastl::move(eastl::shared_ptr<Texture>((Texture *)new GLTexture(_desc)));
    }
}