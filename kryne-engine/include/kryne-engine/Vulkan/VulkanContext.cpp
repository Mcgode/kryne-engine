/**
 * @file
 * @author Max Godefroy
 * @date 16/05/2021.
 */

#ifdef KRYNE_ENGINE_VULKAN


#include "VulkanContext.h"


VulkanContext::VulkanContext(const ivec2 &size)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(size.x, size.y, "Kryne Engine - Vulkan", nullptr, nullptr);

    glfwSetWindowUserPointer(m_window, this);
}


VulkanContext::~VulkanContext()
{
    assertIsMainThread();
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
    return nullptr;
}


unique_ptr<Framebuffer> VulkanContext::makeFramebuffer(const ivec2 &size)
{
    return unique_ptr<Framebuffer>();
}


#endif
