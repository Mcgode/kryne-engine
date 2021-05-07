/**
 * @file
 * @author Max Godefroy
 * @date 06/05/2021.
 */

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

#include "BufferHelpers.hpp"

#include "UniformDescriptor.hpp"

UniformDescriptor::UniformDescriptor(Device *device, const PhysicalDevice &physicalDevice, uint32_t frameCount)
{
    this->device = device;

    DescriptorSetLayoutBinding uboLayoutBinding(0, DescriptorType::eUniformBuffer, 1,
                                                ShaderStageFlagBits::eVertex, nullptr);

    DescriptorSetLayoutCreateInfo layoutInfo({}, 1, &uboLayoutBinding);

    this->setLayout = this->device->createDescriptorSetLayout(layoutInfo);

    const auto bufferSize = sizeof(UBO);
    this->uniformBuffers.resize(frameCount);
    this->uniformBufferMemories.resize(frameCount);

    for (auto i = 0; i < frameCount; i++)
    {
        VulkanHelpers::makeBuffer(bufferSize, BufferUsageFlagBits::eUniformBuffer,
                                  MemoryPropertyFlagBits::eHostVisible | MemoryPropertyFlagBits::eHostCoherent,
                                  this->uniformBuffers[i], this->uniformBufferMemories[i],
                                  this->device, physicalDevice);
    }

    DescriptorPoolSize poolSize(DescriptorType::eUniformBuffer, frameCount);

    DescriptorPoolCreateInfo poolInfo({}, frameCount, 1, &poolSize);
    this->descriptorPool = this->device->createDescriptorPool(poolInfo);

    std::vector<DescriptorSetLayout> layouts(frameCount, this->setLayout);
    DescriptorSetAllocateInfo allocInfo(this->descriptorPool, layouts);

    this->descriptorSets = this->device->allocateDescriptorSets(allocInfo);

    for (auto i = 0; i < frameCount; i++)
    {
        DescriptorBufferInfo bufferInfo(this->uniformBuffers[i], 0, sizeof(UBO));
        WriteDescriptorSet descriptorWrite(this->descriptorSets[i], 0, 0,
                                           1, DescriptorType::eUniformBuffer,
                                           nullptr, &bufferInfo, nullptr);

        this->device->updateDescriptorSets({ descriptorWrite }, {});
    }
}


UniformDescriptor::~UniformDescriptor()
{
    this->device->destroy(this->setLayout);

    for (const auto &m : this->uniformBufferMemories)
        this->device->free(m);

    for (const auto &b : this->uniformBuffers)
        this->device->destroy(b);

    this->device->destroy(this->descriptorPool);
}


void UniformDescriptor::updateUBO(uint32_t index)
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;

    static auto startTime = high_resolution_clock::now();

    auto currentTime = high_resolution_clock::now();

    float time = duration<float>(currentTime - startTime).count();

    UBO ubo {
        glm::rotate(glm::mat4(1), time * glm::radians(90.f), glm::vec3(0, 0, 1)),
        glm::lookAt(glm::vec3(2), glm::vec3(0), glm::vec3(0, 0, 1)),
        glm::perspective(glm::radians(45.f), 0.75f, 0.1f, 10.f)
    };
    ubo.projectionMatrix[1][1] *= -1;

    void *data = this->device->mapMemory(this->uniformBufferMemories[index], 0, sizeof(ubo));
    memcpy(data, &ubo, sizeof(ubo));
    this->device->unmapMemory(this->uniformBufferMemories[index]);
}
