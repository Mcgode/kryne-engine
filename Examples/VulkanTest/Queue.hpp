/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */

#pragma once


#include <vulkan/vulkan.h>
#include <optional>
#include <vector>
#include <set>


namespace VulkanHelpers {

    struct QueueFamilyIndices {

        std::optional<uint32_t> graphicsFamily;

        std::optional<uint32_t> presentFamily;


        [[nodiscard]] bool isComplete() const
        {
            return this->graphicsFamily.has_value() && this->presentFamily.has_value();
        }

        [[nodiscard]] std::set<uint32_t> uniqueFamilies() const
        {
            std::set<uint32_t> r = { this->graphicsFamily.value(), this->presentFamily.value() };
            return r;
        }

    };


    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
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

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport)
                indices.presentFamily = i;

            if (indices.isComplete())
                break;

            i++;
        }

        return indices;
    }

}