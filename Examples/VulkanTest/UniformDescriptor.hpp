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

    explicit UniformDescriptor(Device *device);

    [[nodiscard]] const DescriptorSetLayout &getSetLayout() const { return setLayout; }

    virtual ~UniformDescriptor();

protected:

    Device *device;

    DescriptorSetLayout setLayout;

};


