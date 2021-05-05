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

        static std::array<VertexInputBindingDescription, 1> getBindingDescriptions();

        static std::array<VertexInputAttributeDescription, 2> getAttributeDescriptions();

    };

public:

    explicit VertexBuffer(const PhysicalDevice &physicalDevice, Device *device,
                          const std::vector<Vertex> &vertices);

    PipelineVertexInputStateCreateInfo getPipelineVertexCreationInfo();

    virtual ~VertexBuffer();

protected:

    static uint32_t findMemoryType(uint32_t typeFilter, MemoryPropertyFlags properties,
                                   const PhysicalDevice &physicalDevice);

protected:

    Device *device;

    PipelineVertexInputStateCreateInfo pipelineVertexInfo;

    Buffer buffer;

    DeviceMemory bufferMemory;

};


