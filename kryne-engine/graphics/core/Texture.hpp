/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once

#include <CommonTypes.hpp>

namespace KryneEngine
{
    class Texture
    {
        /**
         * @brief The available texture formats
         */
        enum class Formats
        {
            None = 0,
            
            // Unsigned normalized integer formats
            R8,
            RG8,
            RGB8,
            RGBA8,
            R16,
            RG16,
            RGB16,
            RGBA16,
            R32,
            RG32,
            RGB32,
            RGBA32,
            
            // Float formats
            R16F,
            RG16F,
            RGB16F,
            RGBA16F,
            R32F,
            RG32F,
            RGB32F,
            RGBA32F,

            // Depth + stencil
            D16,
            D24,
            D32F,
            S8,
            D24_S8,
            D32F_S8,

            COUNT
        };

        /**
         * @brief The different supported texture types
         */
        enum class Types
        {
            None = 0,       //!< Uninitialized state. Will fail on creation.
            Texture2D,      //!< Stores image data in a 2D array.
            Texture2DArray, //!< Stores image data as an array of 2D arrays
            Texture3D,      //!< Stores image data as a 3D array.
            CubeMap,        //!< Stores image data as a 2D texture array of size 6 under the hood.
            COUNT
        };

        /**
         * @brief A bitfield for the usages of this particular texture.
         *
         * @details
         * Modern APIs, like Vulkan, expect this information to be passed along during texture creation to allow the
         * driver to optimise image usage in the shaders.
         *
         * Try to set only the relevant usage flags for better performance.
         *
         * @warning Usage flags may not be supported by some formats.
         */
        struct Usage
        {
            /// When set to 1, the texture is expected to be used as an output color attachment
            u16 m_renderTargetColorAttachment : 1;

            /// When set to 1, the texture is expected to be used as an input+output color attachment (though no explicit read, aka sampling, is permitted)
            u16 m_renderTargetBlendableColorAttachment : 1;

            /// When set to 1, the texture is expected to be used as an input+output depth+stencil attachment (though no explicit read is permitted)
            u16 m_renderTargetDepthStencilAttachment : 1;

            /// When set to 1, the texture is expected to be sampled from.
            u16 m_sampleImage: 1;

            /// When set to 1, the texture is expected to be explicitly read and written to (used in compute shader).
            u16 m_readWriteImage: 1;

            /// When set to 1, the texture is expected to be sampled with linear filtering.
            u16 m_linearlyFiltered: 1;
        };

        struct Info
        {
            Types m_type { Types::None };
            Formats m_format { Formats::None };
            Usage m_usage {};
            glm::uvec3 m_size { 0 };
        };


    };
}

