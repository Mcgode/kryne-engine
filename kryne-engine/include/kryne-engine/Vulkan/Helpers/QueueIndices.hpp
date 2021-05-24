/**
 * @file
 * @author max
 * @date 5/16/21.
 */

#pragma once


#include <vulkan/vulkan.hpp>


namespace VulkanHelpers
{

    struct QueueFamilyIndices
    {

        std::optional<uint32_t> m_graphicsFamily;

        std::optional<uint32_t> m_presentFamily;


        [[nodiscard]] bool isComplete() const
        {
            return this->m_graphicsFamily.has_value() && this->m_presentFamily.has_value();
        }

        [[nodiscard]] std::set<uint32_t> uniqueFamilies() const
        {
            std::set<uint32_t> r = {this->m_graphicsFamily.value(), this->m_presentFamily.value() };
            return r;
        }

        static inline QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice &_physicalDevice,
                                                           const vk::SurfaceKHR &_surface)
        {
            QueueFamilyIndices indices {};

            const auto queueFamilies = _physicalDevice.getQueueFamilyProperties();

            int i = 0;
            for (const auto& queueFamily : queueFamilies)
            {
                if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
                    indices.m_graphicsFamily = i;

                vk::Bool32 presentSupport = _physicalDevice.getSurfaceSupportKHR(i, _surface);

                if (presentSupport)
                    indices.m_presentFamily = i;

                if (indices.isComplete())
                    break;

                i++;
            }

            return indices;
        }

    };

}