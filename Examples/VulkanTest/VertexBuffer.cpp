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


VertexBuffer::VertexBuffer(Device *device, const std::vector<Vertex> &vertices)
{
    this->device = device;

    const auto bindingDescriptions = Vertex::getBindingDescriptions();
    const auto attributeDescriptions = Vertex::getAttributeDescriptions();

    pipelineVertexInfo = PipelineVertexInputStateCreateInfo({}, bindingDescriptions, attributeDescriptions);

    BufferCreateInfo createInfo({}, sizeof(Vertex) * vertices.size(), BufferUsageFlagBits::eVertexBuffer,
                                SharingMode::eExclusive);

    this->buffer = this->device->createBuffer(createInfo);
}


PipelineVertexInputStateCreateInfo VertexBuffer::getPipelineVertexCreationInfo()
{
    return this->pipelineVertexInfo;
}


VertexBuffer::~VertexBuffer()
{
    this->device->destroy(this->buffer);
}
