/**
 * @file
 * @author Max Godefroy
 * @date 05/05/2021.
 */

#include "VertexBuffer.hpp"


std::array<VertexInputBindingDescription, 1> VertexBuffer::Vertex::getBindingDescriptions()
{
    std::array<VertexInputBindingDescription, 1> bindingDescription;

    bindingDescription[0] = VertexInputBindingDescription(0, sizeof(Vertex),
                                                          VertexInputRate::eVertex);

    return bindingDescription;
}


std::array<VertexInputAttributeDescription, 2> VertexBuffer::Vertex::getAttributeDescriptions()
{
    std::array<VertexInputAttributeDescription, 2> attributeDescriptions;

    attributeDescriptions[0] = VertexInputAttributeDescription(0, 0, Format::eR32G32Sfloat,
                                                               offsetof(Vertex, position));
    attributeDescriptions[1] = VertexInputAttributeDescription(1, 0, Format::eR32G32B32Sfloat,
                                                               offsetof(Vertex, color));

    return attributeDescriptions;
}


VertexBuffer::VertexBuffer(const PhysicalDevice &physicalDevice, Device *device,
                           const std::vector<Vertex> &vertices)
{
    this->device = device;

    const auto bindingDescriptions = Vertex::getBindingDescriptions();
    const auto attributeDescriptions = Vertex::getAttributeDescriptions();

    pipelineVertexInfo = PipelineVertexInputStateCreateInfo({}, bindingDescriptions, attributeDescriptions);

    BufferCreateInfo bufferInfo({}, sizeof(Vertex) * vertices.size(), BufferUsageFlagBits::eVertexBuffer,
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
