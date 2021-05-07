/**
 * @file
 * @author Max Godefroy
 * @date 05/05/2021.
 */

#include "Utils.hpp"
#include "BufferHelpers.hpp"

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


VertexBuffer::VertexBuffer(const PhysicalDevice &physicalDevice, Device *device, const std::vector<Vertex> &vertices,
                           const CommandPool &commandPool, const Queue &graphicsQueue)
{
    this->device = device;

    this->bindingDescriptions = Vertex::getBindingDescriptions();
    this->attributeDescriptions = Vertex::getAttributeDescriptions();

    this->pipelineVertexInfo = PipelineVertexInputStateCreateInfo({}, bindingDescriptions, attributeDescriptions);

    this->bufferCount = vertices.size();

    const DeviceSize bufferSize = sizeof(Vertex) * vertices.size();

    Buffer stagingBuffer;
    DeviceMemory stagingMemory;

    VulkanHelpers::makeBuffer(bufferSize,
                              BufferUsageFlagBits::eTransferSrc,
                              MemoryPropertyFlagBits::eHostVisible | MemoryPropertyFlagBits::eHostCoherent,
                              stagingBuffer, stagingMemory,
                              this->device, physicalDevice);

    void *data = this->device->mapMemory(stagingMemory, 0, bufferSize);
    memcpy(data, vertices.data(), bufferSize);
    this->device->unmapMemory(stagingMemory);

    VulkanHelpers::makeBuffer(bufferSize,
                              BufferUsageFlagBits::eVertexBuffer | BufferUsageFlagBits::eTransferDst,
                              MemoryPropertyFlagBits::eDeviceLocal,
                              this->buffer, this->bufferMemory,
                              this->device, physicalDevice);

    this->copyBuffer(stagingBuffer, this->buffer, bufferSize, commandPool, graphicsQueue);

    this->device->freeMemory(stagingMemory);
    this->device->destroyBuffer(stagingBuffer);
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


uint32_t VertexBuffer::cmdBind(const CommandBuffer *cmdBuffer, uint32_t count, VertexBuffer **buffers)
{
    uint32_t size = std::numeric_limits<uint32_t >::max();

    std::vector<Buffer> vkBuffers(count);
    std::vector<DeviceSize> offsets(count);

    for (auto i = 0; i < count; i++)
    {
        vkBuffers[i] = buffers[i]->buffer;
        offsets[i] = 0;
        size = std::min(size, buffers[i]->bufferCount);
    }

    cmdBuffer->bindVertexBuffers(0, vkBuffers, offsets);
    cmdBuffer->bindIndexBuffer(buffers[0]->indexBuffer, 0, IndexType::eUint32);

    return size;
}


void VertexBuffer::copyBuffer(const Buffer &src, const Buffer &dst, DeviceSize size, const CommandPool &commandPool,
                              const Queue &graphicsQueue)
{
    CommandBufferAllocateInfo allocInfo(commandPool, CommandBufferLevel::ePrimary, 1);
    const auto commandBuffer = this->device->allocateCommandBuffers(allocInfo)[0];

    CommandBufferBeginInfo beginInfo(CommandBufferUsageFlagBits::eOneTimeSubmit);
    commandBuffer.begin(beginInfo);

    BufferCopy copyRegion(0, 0, size);
    commandBuffer.copyBuffer(src, dst, 1, &copyRegion);

    commandBuffer.end();

    SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VulkanHelpers::assertSuccess(graphicsQueue.submit(1, &submitInfo, {}));
    graphicsQueue.waitIdle();

    this->device->freeCommandBuffers(commandPool, 1, &commandBuffer);
}


void VertexBuffer::setIndex(const PhysicalDevice &physicalDevice, const CommandPool &commandPool,
                            const Queue &graphicsQueue, const std::vector<uint32_t> &indices)
{
    const DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    this->bufferCount = indices.size();

    Buffer stagingBuffer;
    DeviceMemory stagingMemory;

    VulkanHelpers::makeBuffer(bufferSize,
                              BufferUsageFlagBits::eTransferSrc,
                              MemoryPropertyFlagBits::eHostVisible | MemoryPropertyFlagBits::eHostCoherent,
                              stagingBuffer, stagingMemory,
                              this->device, physicalDevice);

    void *data = this->device->mapMemory(stagingMemory, 0, bufferSize);
    memcpy(data, indices.data(), bufferSize);
    this->device->unmapMemory(stagingMemory);

    VulkanHelpers::makeBuffer(bufferSize,
                              BufferUsageFlagBits::eIndexBuffer | BufferUsageFlagBits::eTransferDst,
                              MemoryPropertyFlagBits::eDeviceLocal,
                              this->indexBuffer, this->indexBufferMemory,
                              this->device, physicalDevice);

    this->copyBuffer(stagingBuffer, this->indexBuffer, bufferSize, commandPool, graphicsQueue);

    this->device->freeMemory(stagingMemory);
    this->device->destroyBuffer(stagingBuffer);
}
