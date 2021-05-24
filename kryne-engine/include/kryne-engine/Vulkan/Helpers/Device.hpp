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

    /// @brief KHR swap chain support details
    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR m_capabilities{};

        std::vector<vk::SurfaceFormatKHR> m_formats;

        std::vector<vk::PresentModeKHR> m_presentModes;
    };


    /**
     * @brief An interface for a Vulkan device object
     */
    class Device
    {
    public:

        /// @brief Retrieves the swap chain support details for this device.
        [[nodiscard]] const SwapChainSupportDetails &getSwapChainSupportDetails() const { return m_scSupportDetails; }

        /// @brief Retrieves the family queue indices selected for this device.
        [[nodiscard]] const QueueFamilyIndices &getQueueFamilyIndices() const { return m_queueFamilyIndices; }

        /**
         * @brief Selects and initializes the best suited device
         *
         * @param _instance                  The Vulkan instance of this device.
         * @param _surface                   The Vulkan screen surface to present to.
         * @param _requiredDeviceExtensions  The extensions required for the device to be suitable.
         *
         * @return The newly initialized device
         */
        static Device *selectDevice(const vk::Instance &_instance,
                                    const vk::SurfaceKHR &_surface,
                                    const std::vector<const char *> &_requiredDeviceExtensions,
                                    const std::vector<const char *> &_validationLayers);

    private:

        /**
         * @brief Initializes the device and finishes retrieval of device data
         *
         * @param _physicalDevice   The selected physical device.
         * @param _surface          The screen surface to present to.
         */
        Device(const vk::PhysicalDevice &_physicalDevice,
               const vk::SurfaceKHR &_surface,
               const std::vector<const char *> &_requiredDeviceExtensions,
               const std::vector<const char *> &_validationLayers);

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
        static SwapChainSupportDetails querySwapChainDetails(const vk::PhysicalDevice &_physicalDevice,
                                                             const vk::SurfaceKHR &_surface);

        /// The underlying physical device.
        vk::PhysicalDevice m_physicalDevice;

        /// The Vulkan device we're working with.
        vk::Device m_device;

        /// The device surface we'll draw onto.
        const vk::SurfaceKHR &m_surface;

        /// Device swap chain support details.
        SwapChainSupportDetails m_scSupportDetails;

        /// The indices of the queues.
        QueueFamilyIndices m_queueFamilyIndices;

    };

};