/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once
#include <graphics/core/GraphicsApiContext.hpp>

namespace KryneEngine
{
    class GlApiContext: public GraphicsApiContext
    {
    public:
        eastl::shared_ptr<Texture> &&CreateTexture(const Texture::Description &_desc) override;
    };
}


