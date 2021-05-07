/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */

#pragma once


#include <vulkan/vulkan.h>
#include <optional>
#include <vector>


namespace VulkanHelpers {

    struct QueueFamilyIndices {

        std::optional<uint32_t> graphicsFamily;


        [[nodiscard]] bool isComplete() const
        {
            return this->graphicsFamily.has_value();
        }

    };


    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices {};

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            if (indices.isComplete())
                break;

            i++;
        }

        return indices;
    }

}