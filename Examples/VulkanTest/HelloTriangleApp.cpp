/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */


#include <cstring>
#include "DebuggingUtils.hpp"
#include "Queue.hpp"

#include "HelloTriangleApp.hpp"



const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifndef NDEBUG

#define USE_DEBUG_LAYER

#endif


namespace {

    bool checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions)
            requiredExtensions.erase(extension.extensionName);

        return requiredExtensions.empty();
    }


    struct SwapChainSupportDetails {

        VkSurfaceCapabilitiesKHR capabilities;

        std::vector<VkSurfaceFormatKHR> formats;

        std::vector<VkPresentModeKHR> presentModes;

    };

    SwapChainSupportDetails querySwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatsCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatsCount, nullptr);

        if (formatsCount != 0) {
            details.formats.resize(formatsCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatsCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }


    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto &format : availableFormats)
        {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
                return format;
        }
        return availableFormats[0];
    }


    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return availablePresentMode;
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }


    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow *window)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;

        else
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }


    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        bool extensionSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionSupported)
        {
            auto details = querySwapChainDetails(device, surface);
            swapChainAdequate = !details.formats.empty() && !details.presentModes.empty();
        }

        auto indices = VulkanHelpers::findQueueFamilies(device, surface);

        return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
               deviceFeatures.geometryShader &&
               indices.isComplete() &&
               extensionSupported &&
               swapChainAdequate;
    }


    std::vector<const char*> getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef USE_DEBUG_LAYER

        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

#endif

        return extensions;
    }

}


void HelloTriangleApp::run()
{
    this->initVulkan();
    this->mainLoop();
    this->cleanup();
}


void HelloTriangleApp::initVulkan()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    this->window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    this->createInstance();
    this->setupDebugMessenger();
    this->createSurface();
    this->pickPhysicalDevice();
    this->initLogicalDevice();
    this->createSwapChain();
    this->setUpImageViews();
    this->createGraphicsPipeline();
}


void HelloTriangleApp::mainLoop()
{
    while (!glfwWindowShouldClose(this->window))
    {
        glfwPollEvents();
    }
}


void HelloTriangleApp::cleanup()
{
    for (const auto &imgView : this->swapChainImageViews)
        vkDestroyImageView(this->device, imgView, nullptr);

    vkDestroySwapchainKHR(this->device, this->swapChain, nullptr);

    vkDestroyDevice(this->device, nullptr);

    vkDestroySurfaceKHR(this->instance, this->surface, nullptr);

#ifdef USE_DEBUG_LAYER

    Utils::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

#endif

    vkDestroyInstance(this->instance, nullptr);

    glfwDestroyWindow(this->window);

    glfwTerminate();
}


void HelloTriangleApp::createInstance()
{
#ifdef USE_DEBUG_LAYER

    if (!HelloTriangleApp::checkValidationLayerSupport())
        throw std::runtime_error("Validation layer requested, but unavailable");

#endif

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions:\n";
    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    const auto requiredExtensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

#ifdef USE_DEBUG_LAYER

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    Utils::populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = &debugCreateInfo;

#else

        createInfo.enabledLayerCount = 0;

#endif

    if (vkCreateInstance(&createInfo, nullptr, &this->instance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create VK instance");
}


bool HelloTriangleApp::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}


void HelloTriangleApp::setupDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;

    Utils::populateDebugMessengerCreateInfo(createInfo);
    createInfo.pUserData = nullptr; // Optional

    if (Utils::CreateDebugUtilsMessengerEXT(this->instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
        throw std::runtime_error("Unable to set up debug messenger");
}


void HelloTriangleApp::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("No available Vulkan-compatible devices");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(this->instance, &deviceCount, devices.data());

    for (const auto d : devices)
    {
        if (isDeviceSuitable(d, this->surface))
        {
            this->physicalDevice = d;
            break;
        }
    }

    if (this->physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("No suitable VK device");
}


void HelloTriangleApp::initLogicalDevice()
{
    auto indices = VulkanHelpers::findQueueFamilies(this->physicalDevice, this->surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    for (const auto familyIndex : indices.uniqueFamilies())
    {
        VkDeviceQueueCreateInfo queueCreateInfo {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = familyIndex;
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    vkGetPhysicalDeviceFeatures(this->physicalDevice, &deviceFeatures);

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = queueCreateInfos.size();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    createInfo.enabledExtensionCount = deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

#ifdef USE_DEBUG_LAYER

    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();

#else

    createInfo.enabledLayerCount = 0;

#endif

    if (vkCreateDevice(this->physicalDevice, &createInfo, nullptr, &this->device) != VK_SUCCESS)
        throw std::runtime_error("Unable to create device object");

    vkGetDeviceQueue(this->device, indices.presentFamily.value(), 0, &this->presentQueue);
}


void HelloTriangleApp::createSurface()
{
    if (glfwCreateWindowSurface(this->instance, this->window, nullptr, &this->surface) != VK_SUCCESS)
        throw std::runtime_error("Unable to create window surface");
}


void HelloTriangleApp::createSwapChain()
{
    auto details = querySwapChainDetails(this->physicalDevice, this->surface);

    auto surfaceFormat = chooseSwapSurfaceFormat(details.formats);
    auto presentMode = chooseSwapPresentMode(details.presentModes);
    auto extent = chooseSwapExtent(details.capabilities, this->window);

    uint32_t imageCount = details.capabilities.minImageCount + 1;

    if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
        imageCount = details.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = this->surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto indices = VulkanHelpers::findQueueFamilies(this->physicalDevice, this->surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = true;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(this->device, &createInfo, nullptr, &this->swapChain) != VK_SUCCESS)
        throw std::runtime_error("Unable to create swap chain");

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    this->swapChainExtent = extent;
    this->swapChainImageFormat = surfaceFormat.format;

}


void HelloTriangleApp::setUpImageViews()
{
    this->swapChainImageViews.resize(this->swapChainImages.size());

    for (auto i = 0; i < this->swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = this->swapChainImages[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = this->swapChainImageFormat;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        // Color target
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

        // No mip map level
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;

        // No array layer
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(this->device, &createInfo, nullptr, &this->swapChainImageViews[i]) != VK_SUCCESS)
            throw std::runtime_error("Unable to initialize image view");
    }
}


void HelloTriangleApp::createGraphicsPipeline()
{

}
