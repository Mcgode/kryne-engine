/**
 * @file
 * @author Max Godefroy
 * @date 06/05/2021.
 */

#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/mat4x4.hpp>


using namespace vk;


class UniformDescriptor {

public:

    struct UBO {

        glm::mat4 worldMatrix;

        glm::mat4 viewMatrix;

        glm::mat4 projectionMatrix;

    };

public:

    explicit UniformDescriptor(Device *device, const PhysicalDevice &physicalDevice, uint32_t frameCount);

    [[nodiscard]] const DescriptorSetLayout &getSetLayout() const { return setLayout; }

    void updateUBO(uint32_t index);

    [[nodiscard]] const DescriptorSet &getDescriptorSet(uint32_t index) const { return this->descriptorSets[index]; }

    virtual ~UniformDescriptor();

protected:

    Device *device;

    DescriptorSetLayout setLayout;

    std::vector<Buffer> uniformBuffers;

    std::vector<DeviceMemory> uniformBufferMemories;

    DescriptorPool descriptorPool;

    std::vector<DescriptorSet> descriptorSets;

};


