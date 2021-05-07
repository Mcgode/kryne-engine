/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */

#pragma once

#include <vulkan/vulkan.hpp>


namespace VulkanHelpers {

    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties,
                            const vk::PhysicalDevice &physicalDevice);

    void makeBuffer(vk::DeviceSize bufferSize, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
                    vk::Buffer &buffer, vk::DeviceMemory &memory, const vk::Device *device,
                    const vk::PhysicalDevice &physicalDevice);

}


