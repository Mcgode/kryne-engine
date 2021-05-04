/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */

#pragma once

#pragma message("Queue included")


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


    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

}