/**
 * @file
 * @author Max Godefroy
 * @date 05/05/2021.
 */

#include "VertexBuffer.hpp"


std::vector<VertexInputBindingDescription> VertexBuffer::Vertex::getBindingDescriptions()
{
    std::vector<VertexInputBindingDescription> bindingDescription = {
            { 0, sizeof(Vertex), VertexInputRate::eVertex }
    };

    return bindingDescription;
}


std::vector<VertexInputAttributeDescription> VertexBuffer::Vertex::getAttributeDescriptions()
{
    std::vector<VertexInputAttributeDescription> attributeDescriptions = {
            { 0, 0, Format::eR32G32Sfloat, (uint32_t) offsetof(Vertex, position) },
            { 1, 0, Format::eR32G32B32Sfloat, (uint32_t) offsetof(Vertex, color) },
    };

    return attributeDescriptions;
}


VertexBuffer::VertexBuffer(const PhysicalDevice &physicalDevice, Device *device,
                           const std::vector<Vertex> &vertices)
{
    this->device = device;

    this->bindingDescriptions = Vertex::getBindingDescriptions();
    this->attributeDescriptions = Vertex::getAttributeDescriptions();

    this->pipelineVertexInfo = PipelineVertexInputStateCreateInfo({}, bindingDescriptions, attributeDescriptions);

    bufferInfo = BufferCreateInfo({}, sizeof(Vertex) * vertices.size(), BufferUsageFlagBits::eVertexBuffer,
                                  SharingMode::eExclusive);

    this->buffer = this->device->createBuffer(bufferInfo);

    auto memRequirements = this->device->getBufferMemoryRequirements(this->buffer);

    MemoryAllocateInfo allocateInfo(memRequirements.size);
    allocateInfo.memoryTypeIndex = VertexBuffer::findMemoryType(memRequirements.memoryTypeBits,
                                                                MemoryPropertyFlagBits::eHostVisible | MemoryPropertyFlagBits::eHostCoherent,
                                                                physicalDevice);

    this->bufferMemory = this->device->allocateMemory(allocateInfo);
    this->device->bindBufferMemory(this->buffer, this->bufferMemory, 0);

    void *data = this->device->mapMemory(this->bufferMemory, 0, bufferInfo.size);
    memcpy(data, vertices.data(), bufferInfo.size);
    this->device->unmapMemory(this->bufferMemory);
}


PipelineVertexInputStateCreateInfo VertexBuffer::getPipelineVertexCreationInfo()
{
    return this->pipelineVertexInfo;
}


VertexBuffer::~VertexBuffer()
{
    this->device->destroy(this->buffer);
    this->device->freeMemory(this->bufferMemory);
}


uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, MemoryPropertyFlags properties,
                                      const PhysicalDevice &physicalDevice)
{

    const auto memProperties = physicalDevice.getMemoryProperties();

    for (auto i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & 1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }

    throw std::runtime_error("No suitable memory type");
}


uint32_t VertexBuffer::cmdBind(const CommandBuffer *cmdBuffer, uint32_t count, VertexBuffer **buffers)
{
    uint32_t size = 0;

    std::vector<Buffer> vkBuffers(count);
    std::vector<DeviceSize> offsets(count);

    for (auto i = 0; i < count; i++)
    {
        vkBuffers[i] = buffers[i]->buffer;
        offsets[i] = 0;
        size += buffers[i]->bufferInfo.size / sizeof(Vertex);
    }

    cmdBuffer->bindVertexBuffers(0, vkBuffers, offsets);

    return size;
}
