/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once

#include <EASTL/unique_ptr.h>
#include <EASTL/vector.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <ke/common/Assert.hpp>
#include <ke/common/CommonTypes.hpp>

namespace KryneEngine
{
    class Texture
    {
    public:
        /**
         * A format for storing raw image data, with the first vector scope for array layer, and the second scope for
         * individual mip data.
         */
        using ImageData = eastl::vector<eastl::vector<void*>>;

        /**
         * @brief The available texture formats
         */
        enum class Formats
        {
            None = 0,

            RedFormats = None,
                R8,
                R16,
                R16F,
                R32F,
            RedFormatsEnd = R32F,

            RgFormats = RedFormatsEnd,
                RG8,
                RG16,
                RG32,
                RG16F,
                RG32F,
            RgFormatsEnd = RG32F,

            RgbFormats = RgFormatsEnd,
                RGB8,
                RGB16,
                RGB32,
                RGB16F,
                RGB32F,
            RgbFormatsEnd = RGB32F,

            RgbaFormats = RgbFormatsEnd,
                RGBA8,
                RGBA16,
                RGBA32,
                RGBA16F,
                RGBA32F,
            RgbaFormatsEnd = RGBA32F,

            DepthFormats = RgbaFormatsEnd,
                D16,
                D24,
                D32F,
            DepthFormatsEnd = D32F,

            StencilFormats = DepthFormatsEnd,
                S8,
            StencilFormatsEnd = S8,

            DepthStencilFormats = StencilFormatsEnd,
                D24_S8,
                D32F_S8,
            DepthStencilFormatsEnd = D32F_S8,

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

        enum class Wrap
        {
            ClampToEdge = 0,    //!< Last pixel in that direction will be repeated infinitely
            Repeat,             //!< Repeat by tiling. Can have hard edges
            MirrorRepeat,       //!< Repeat, applying mirroring. Always matching edges
            ClampToBorder       //!< Every outside pixel is the color of the border color
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
            /// When set to true, the texture is expected to be used as an output color attachment
            bool m_renderTargetColorAttachment : 1;

            /// When set to true, the texture is expected to be used as an input+output color attachment (though no explicit read, aka sampling, is permitted)
            bool m_renderTargetBlendableColorAttachment : 1;

            /// When set to true, the texture is expected to be used as an input+output depth+stencil attachment (though no explicit read is permitted)
            bool m_renderTargetDepthStencilAttachment : 1;

            /// When set to true, the texture is expected to be sampled from.
            bool m_sampleImage: 1;

            /// When set to true, the texture is expected to be explicitly read and written to (used in compute shader).
            bool m_readWriteImage: 1;

            /// When set to true, the texture is expected to be sampled with linear filtering.
            bool m_linearlyFiltered: 1;
        };

        /**
         * @brief The specifications description of a texture.
         */
        struct Description
        {
            Types m_type { Types::None };
            Formats m_format { Formats::None };
            Usage m_usage {};
            struct {
                bool m_linearMinify = false;
                bool m_linearMagnify = false;
                bool m_linearMips = false;
            } m_filtering;
            glm::vec4 m_borderColor { 0 };
            glm::vec<3, Wrap> m_wrapping;
            glm::uvec3 m_size { 0 };
            u8 m_mipCount { 0 };
        };

        /**
         * A memory helper for the APIs able to pass memory around
         */
        struct Memory
        {
            /// Stores whether this memory is an owner or lessee.
            const bool m_isLessee;

            /// Keeps track of the lessees, for memory tracking purposes
            eastl::vector<Memory*> m_lessees {};

            /// The original owner of this memory, if leased.
            const Memory* m_owner = nullptr;

            /// The texture using this memory.
            Texture* m_textureUser = nullptr;

            /// Allows the API to store an API-level memory handler, if the need arises
            void* m_apiMemoryHandle = nullptr;

        public:

            /**
             * @brief Creates an "owner" memory helper
             *
             * @param _apiHandle The underlying memory handle.
             */
            explicit Memory(void* _apiHandle): m_isLessee(false), m_apiMemoryHandle(_apiHandle) {}

            /**
             * @brief Instantiates a handle representing a lease of this memory.
             */
            [[nodiscard]] eastl::unique_ptr<Memory>&& Lease()
            {
                Assert(!this->m_isLessee, "Only the owner can lease");
                return eastl::move(eastl::unique_ptr<Memory>(new Memory(this)));
            }

        protected:

            /**
             * @brief Initializes a leased memory handle
             *
             * @param _owner    The original memory owner
             */
            explicit Memory(Memory* _owner)
                : m_isLessee(true)
                , m_owner(_owner)
                , m_apiMemoryHandle(_owner->m_apiMemoryHandle)
            {
                _owner->m_lessees.push_back(this);
            }
        };


        /// @brief Retrieves this texture's information, such as format or size.
        [[nodiscard]] const Description& GetDescription() const { return m_description; }

        /**
         * @brief Updates the underlying texture access header (and potentially memory) to fit this new description.
         *
         * @param _newDescription   The new description for this texture.
         */
        virtual void UpdateDescription(const Description& _newDescription) = 0;

        /**
         * @brief Forces this texture to acquire memory that it will own
         *
         * @param _imageData The image data to fill the memory with. Optional, memory can be kept to nullptr.
         *
         * @warning
         * It is expected from the implementations to handle incomplete data, as it is legal for the user not
         * to fill in all of it.
         */
         virtual void AcquireMemory(ImageData* _imageData = nullptr) = 0;

        /**
         * @brief Forces this texture to release its memory (thus discarding its memory helper)
         */
        virtual void FreeMemory() = 0;

    protected:

        /**
         * @brief A constructor for inheriting classes
         *
         * @param _createInfo     The image info used to create the texture
         * @param _memoryHelper   The initial memory space helper of this texture. Can be null.
         */
        explicit Texture(const Description& _createInfo, eastl::unique_ptr<Memory>&& _memoryHelper = nullptr)
            : m_description(_createInfo)
            , m_memory(eastl::move(_memoryHelper))
        {
        }

        /// Tracks whether this texture has some memory allocated.
        eastl::unique_ptr<Memory> m_memory;

        /// The information used to create this texture.
        Description m_description;
    };
}
