/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */

#include "BufferHelpers.hpp"

uint32_t VulkanHelpers::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties,
                                       const vk::PhysicalDevice &physicalDevice)
{
    const auto memProperties = physicalDevice.getMemoryProperties();

    for (auto i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & 1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }

    throw std::runtime_error("No suitable memory type");
}


void VulkanHelpers::makeBuffer(vk::DeviceSize bufferSize, vk::BufferUsageFlags usage,
                               vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &memory,
                               const vk::Device *device, const vk::PhysicalDevice &physicalDevice)
{
    const auto bufferInfo = vk::BufferCreateInfo({}, bufferSize, usage,
                                                 vk::SharingMode::eExclusive);

    buffer = device->createBuffer(bufferInfo);

    const auto memRequirements = device->getBufferMemoryRequirements(buffer);

    vk::MemoryAllocateInfo allocInfo(memRequirements.size);
    allocInfo.memoryTypeIndex = VulkanHelpers::findMemoryType(memRequirements.memoryTypeBits, properties,
                                                              physicalDevice);

    memory = device->allocateMemory(allocInfo);

    device->bindBufferMemory(buffer, memory, 0);
}
