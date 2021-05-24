/**
 * @file
 * @author max
 * @date 5/16/21.
 */

#pragma once


#include <vector>

#include "QueueIndices.hpp"


namespace VulkanHelpers
{

    /**
     * @brief An interface for a Vulkan device object
     */
    class Device
    {
    public:

        /// @brief KHR swap chain support details
        struct SwapChainSupportDetails
        {
            vk::SurfaceCapabilitiesKHR m_capabilities {};

            std::vector<vk::SurfaceFormatKHR> m_formats;

            std::vector<vk::PresentModeKHR> m_presentModes;
        };

    public:

        [[nodiscard]] const SwapChainSupportDetails &getScSupportDetails() const { return m_scSupportDetails; }

        /**
         * @brief Selects and initializes the best suited device
         *
         * @param _instance                  The Vulkan instance of this device.
         * @param _requiredDeviceExtensions  The extensions required for the device to be suitable.
         *
         * @return The newly initialized device
         */
        static Device *selectDevice(const vk::Instance &_instance, const vk::SurfaceKHR &_surface,
                                    const std::vector<const char *> &_requiredDeviceExtensions);

    private:

        Device(const vk::PhysicalDevice &_physicalDevice, const vk::SurfaceKHR &_surface);

    private:

        /**
         * @brief Checks if a given device supports all the required extensions.
         *
         * @param _physicalDevice               The physical device to check.
         * @param _requiredDeviceExtensions     The extensions required for the device to be suitable.
         *
         * @return true if the device supports all the required extensions.
         */
        static bool checkRequiredExtensionsSupport(const vk::PhysicalDevice &_physicalDevice,
                                                   const std::vector<const char *> &_requiredDeviceExtensions);

        /**
         * @brief Retrieves the swap chain properties
         *
         * @param _physicalDevice   The physical device to retrieve the data for.
         * @param _surface          The surface which we'll render to.
         */
        static SwapChainSupportDetails querySwapChainDetails(const vk::PhysicalDevice &_physicalDevice, const vk::SurfaceKHR &_surface);

    private:

        /// The underlying physical device.
        vk::PhysicalDevice m_physicalDevice;

        /// The Vulkan device we're working with.
        vk::Device m_device;

        /// The device surface we'll draw onto.
        const vk::SurfaceKHR &m_surface;

        /// Device swap chain support details.
        SwapChainSupportDetails m_scSupportDetails;

    };

};