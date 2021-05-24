/**
 * @file
 * @author max
 * @date 5/16/21.
 */

#include <set>

#include "Device.hpp"


using namespace VulkanHelpers;


Device::Device(const vk::PhysicalDevice &_physicalDevice, const vk::SurfaceKHR &_surface) :
    m_physicalDevice(_physicalDevice), m_surface(_surface)
{
    m_scSupportDetails = Device::querySwapChainDetails(m_physicalDevice, m_surface);
}


Device *Device::selectDevice(const vk::Instance &_instance, const vk::SurfaceKHR &_surface,
                             const std::vector<const char *> &_requiredDeviceExtensions)
{
    std::vector<vk::PhysicalDevice> physicalDevices = _instance.enumeratePhysicalDevices();

    vk::PhysicalDevice *pickedDevice = nullptr;
    uint32_t score = -1;

    for (vk::PhysicalDevice &device: physicalDevices)
    {
        if (!Device::checkRequiredExtensionsSupport(device, _requiredDeviceExtensions))
        {
            continue;
        }

        uint32_t currentScore = 0;

        const vk::PhysicalDeviceProperties properties = device.getProperties();
        const vk::PhysicalDeviceFeatures features = device.getFeatures();

        currentScore += properties.limits.maxImageDimension2D;

        switch (properties.deviceType)
        {
            case vk::PhysicalDeviceType::eDiscreteGpu:
                currentScore *= 2;
                break;
            case vk::PhysicalDeviceType::eCpu:
                currentScore /= 2;
                break;
            case vk::PhysicalDeviceType::eVirtualGpu:
                currentScore = (currentScore * 3)  / 2;
                break;
            default:
                break;
        }

        if (currentScore > score)
        {
            pickedDevice = &device;
            score = currentScore;
        }
    }

    if (score <= 0)
    {
        throw std::runtime_error("No suitable device to pick from");
    }

    auto device = new Device(*pickedDevice, _surface);

    return device;
}


bool Device::checkRequiredExtensionsSupport(const vk::PhysicalDevice &_physicalDevice,
                                            const std::vector<const char *> &_requiredDeviceExtensions)
{
    auto availableExtensions = _physicalDevice.enumerateDeviceExtensionProperties();

    std::set<std::string> requiredExtensions(_requiredDeviceExtensions.begin(),
                                             _requiredDeviceExtensions.end());

    for (const auto& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}


Device::SwapChainSupportDetails Device::querySwapChainDetails(const vk::PhysicalDevice &_physicalDevice,
                                                              const vk::SurfaceKHR &_surface)
{
    SwapChainSupportDetails details;

    details.m_capabilities = _physicalDevice.getSurfaceCapabilitiesKHR(_surface);
    details.m_formats = _physicalDevice.getSurfaceFormatsKHR(_surface);
    details.m_presentModes = _physicalDevice.getSurfacePresentModesKHR(_surface);

    return details;
}
