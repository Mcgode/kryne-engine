/**
 * @file
 * @author Max Godefroy
 * @date 06/05/2021.
 */

#include "UniformDescriptor.hpp"

UniformDescriptor::UniformDescriptor(Device *device)
{
    this->device = device;

    DescriptorSetLayoutBinding uboLayoutBinding(0, DescriptorType::eUniformBuffer, 1,
                                                ShaderStageFlagBits::eVertex, nullptr);

    DescriptorSetLayoutCreateInfo layoutInfo({}, 1, &uboLayoutBinding);

    this->setLayout = this->device->createDescriptorSetLayout(layoutInfo);
}


UniformDescriptor::~UniformDescriptor()
{
    this->device->destroy(this->setLayout);
}
