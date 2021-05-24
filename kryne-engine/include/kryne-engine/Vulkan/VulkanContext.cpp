/**
 * @file
 * @author Max Godefroy
 * @date 16/05/2021.
 */

#ifdef KRYNE_ENGINE_VULKAN


#include "VulkanRenderer.h"

#include "VulkanContext.hpp"


VulkanContext::VulkanContext(const ivec2 &size)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(size.x, size.y, "Kryne Engine - Vulkan", nullptr, nullptr);

    glfwSetWindowUserPointer(m_window, this);

    this->initInstance();

    {
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Unable to initialize GLFW VK surface");
        }
        m_surface = surface;
    }

    m_device = unique_ptr<VulkanHelpers::Device>(VulkanHelpers::Device::selectDevice(m_instance, m_surface,
                                                                                     VulkanContext::requiredDeviceExtensions()));

//    m_renderer = make_unique<VulkanRenderer>();
}


void VulkanContext::initInstance()
{
    vk::ApplicationInfo appInfo("Kryne engine",
                                VK_MAKE_VERSION(1, 0, 0),
                                "Kryne Engine",
                                VK_MAKE_VERSION(1, 0, 0),
                                VK_API_VERSION_1_1);

    auto extensions = vk::enumerateInstanceExtensionProperties();

    vk::InstanceCreateInfo createInfo({}, &appInfo,
                                      0, nullptr,
                                      VulkanContext::requiredDeviceExtensions().size(),
                                      VulkanContext::requiredDeviceExtensions().data());

    m_instance = vk::createInstance(createInfo);
}


VulkanContext::~VulkanContext()
{
    assertIsMainThread();

    m_device.reset();
    m_instance.destroy();

    glfwTerminate();
}


bool VulkanContext::shouldStop()
{
    return glfwWindowShouldClose(m_window);
}


void VulkanContext::stop()
{
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}


void VulkanContext::endFrame()
{
    glfwPollEvents();
}


PlayerInput *VulkanContext::getPlayerInput()
{
    return nullptr;
}


LoopRenderer *VulkanContext::getRenderer()
{
    return m_renderer.get();
}


unique_ptr<Framebuffer> VulkanContext::makeFramebuffer(const ivec2 &size)
{
    return unique_ptr<Framebuffer>();
}


#endif
