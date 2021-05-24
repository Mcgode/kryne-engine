/**
 * @file
 * @author Max Godefroy
 * @date 16/05/2021.
 */

#pragma once

#ifdef KRYNE_ENGINE_VULKAN


#include "../Core/GraphicContext/GraphicContext.h"
#include "Helpers/Device.hpp"


class VulkanRenderer;


class VulkanContext : public GraphicContext
{
public:

    /**
     * @brief Initialize a Vulkan context
     *
     * @param size  The initial window size for the Vulkan renderer
     */
    explicit VulkanContext(const ivec2 &size = ivec2(1280, 720));

    virtual ~VulkanContext();

    // Override
    bool shouldStop() override;

    // Override
    void stop() override;

    // Override
    void endFrame() override;

    // Override
    PlayerInput *getPlayerInput() override;

    // Override
    LoopRenderer *getRenderer() override;

    // Override
    unique_ptr<Framebuffer> makeFramebuffer(const ivec2 &size) override;

protected:

    void initInstance();

protected:

    GLFWwindow* m_window;

    unique_ptr<VulkanRenderer> m_renderer;

    vk::Instance m_instance;

    vk::SurfaceKHR m_surface;

    std::unique_ptr<VulkanHelpers::Device> m_device;

protected:

    /// The list of the extensions required to use a certain device
    static inline std::vector<const char *> requiredDeviceExtensions() {
        return {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };
    }

};

#endif
