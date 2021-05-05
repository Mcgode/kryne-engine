/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */


#include <cstring>
#include <fstream>
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


    std::vector<char> readFile(const char *filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error("Unable to open file");

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }


    VkShaderModule createShaderModule(const std::vector<char> &code, const VkDevice &device)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
            throw std::runtime_error("failed to create shader module!");

        return shaderModule;
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
//    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    this->window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    glfwSetWindowUserPointer(this->window, this);
    glfwSetFramebufferSizeCallback(this->window, HelloTriangleApp::framebufferResizeCallback);

    this->createInstance();
    this->setupDebugMessenger();
    this->createSurface();
    this->pickPhysicalDevice();

    this->initLogicalDevice();
    this->device_hpp = Device(this->device);

    this->createCommandPool();
    this->commandPool_hpp = CommandPool(this->commandPool);

    const std::vector<VertexBuffer::Vertex> vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    auto sc = new SwapChain(PhysicalDevice(this->physicalDevice),
                            SurfaceKHR(this->surface),
                            this->window,
                            &this->commandPool_hpp,
                            &this->device_hpp,
                            vertices);
    this->swapChain.reset(sc);

    this->createSyncObjects();
}


void HelloTriangleApp::mainLoop()
{
    while (!glfwWindowShouldClose(this->window))
    {
        glfwPollEvents();
        this->drawFrame();
    }

    vkDeviceWaitIdle(this->device);
}


void HelloTriangleApp::cleanup()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(this->device, this->renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(this->device, this->imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(this->device, this->inFlightFences[i], nullptr);
    }

    this->swapChain.reset();

    vkDestroyCommandPool(this->device, this->commandPool, nullptr);

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

    vkGetDeviceQueue(this->device, indices.graphicsFamily.value(), 0, &this->graphicsQueue);
    vkGetDeviceQueue(this->device, indices.presentFamily.value(), 0, &this->presentQueue);
}


void HelloTriangleApp::createSurface()
{
    if (glfwCreateWindowSurface(this->instance, this->window, nullptr, &this->surface) != VK_SUCCESS)
        throw std::runtime_error("Unable to create window surface");
}


void HelloTriangleApp::createCommandPool()
{
    auto queueFamilyIndices = VulkanHelpers::findQueueFamilies(this->physicalDevice, this->surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(this->device, &poolInfo, nullptr, &this->commandPool) != VK_SUCCESS)
        throw std::runtime_error("Unable to create command pool");
}


void HelloTriangleApp::createSyncObjects()
{
    this->imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    this->renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    this->inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    SemaphoreCreateInfo semaphoreInfo;

    FenceCreateInfo fenceInfo(FenceCreateFlagBits::eSignaled);

    for (auto i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        this->imageAvailableSemaphores[i] = this->device_hpp.createSemaphore(semaphoreInfo);
        this->renderFinishedSemaphores[i] = this->device_hpp.createSemaphore(semaphoreInfo);
        this->inFlightFences[i] = this->device_hpp.createFence(fenceInfo);
    }
}


void HelloTriangleApp::drawFrame()
{
    if (this->framebufferResized)
    {
        std::cout << "Frame resized, will recreate swap chain" << std::endl;
        this->resetSwapChain();
        this->framebufferResized = false;
    }

    if   (!this->swapChain->draw(&this->imageAvailableSemaphores[this->currentFrame],
                                 &this->renderFinishedSemaphores[this->currentFrame],
                                 &this->inFlightFences[this->currentFrame],
                                 this->graphicsQueue,
                                 this->presentQueue))
    {
        std::cout << "Swap chain is out of date, will recreate" << std::endl;
        this->resetSwapChain();
    }

    this->currentFrame = (this->currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


void HelloTriangleApp::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto app = (HelloTriangleApp *) glfwGetWindowUserPointer(window);
    app->framebufferResized = true;
}

void HelloTriangleApp::resetSwapChain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);

    while (width == 0 || height == 0)
    {
        glfwWaitEvents();
        glfwGetFramebufferSize(window, &width, &height);
    }

    this->device_hpp.waitIdle();

    // Make sure previous swap chain is already deleted.
    this->swapChain.reset();

    const std::vector<VertexBuffer::Vertex> vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    auto sc = new SwapChain(PhysicalDevice(this->physicalDevice), SurfaceKHR(this->surface),
                            this->window, &this->commandPool_hpp, &this->device_hpp, vertices);
    this->swapChain.reset(sc);
}
