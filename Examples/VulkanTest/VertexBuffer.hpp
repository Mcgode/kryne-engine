/**
 * @file
 * @author Max Godefroy
 * @date 05/05/2021.
 */

#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace vk;


class VertexBuffer {

public:

    struct Vertex {

        glm::vec2 position;

        glm::vec3 color;

        static std::vector<VertexInputBindingDescription> getBindingDescriptions();

        static std::vector<VertexInputAttributeDescription> getAttributeDescriptions();

    };

public:

    explicit VertexBuffer(const PhysicalDevice &physicalDevice, Device *device,
                          const std::vector<Vertex> &vertices);

    PipelineVertexInputStateCreateInfo getPipelineVertexCreationInfo();

    static uint32_t cmdBind(const CommandBuffer *cmdBuffer, uint32_t count, VertexBuffer **buffers);

    inline static uint32_t cmdBind(const CommandBuffer &cmdBuffer, const ArrayProxy<VertexBuffer *> &buffers)
    {
        return cmdBind(&cmdBuffer, buffers.size(), buffers.data());
    }

    virtual ~VertexBuffer();

protected:

    static uint32_t findMemoryType(uint32_t typeFilter, MemoryPropertyFlags properties,
                                   const PhysicalDevice &physicalDevice);

protected:

    Device *device;

    std::vector<VertexInputBindingDescription> bindingDescriptions;

    std::vector<VertexInputAttributeDescription> attributeDescriptions;

    PipelineVertexInputStateCreateInfo pipelineVertexInfo;

    BufferCreateInfo bufferInfo;

    Buffer buffer;

    DeviceMemory bufferMemory;

};


