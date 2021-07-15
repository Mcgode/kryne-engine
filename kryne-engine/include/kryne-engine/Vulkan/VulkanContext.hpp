/**
 * @file
 * @author Max Godefroy
 * @date 16/05/2021.
 */

#pragma once


#include "../Core/GraphicContext/GraphicContext.h"
#include "Helpers/Device.hpp"


class VulkanRenderer;


#if !defined(NDEBUG)
#define VALIDATION_LAYER_DEFAULT true
#else
#define VALIDATION_LAYER_DEFAULT false
#endif


class VulkanContext : public GraphicContext
{
public:

    /**
     * @brief Initialize a Vulkan context
     *
     * @param size  The initial window size for the Vulkan renderer
     */
    explicit VulkanContext(const ivec2 &size = ivec2(1280, 720), bool useValidationLayers = VALIDATION_LAYER_DEFAULT);

    // Destructor
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

    /// The underlying GLFW window
    GLFWwindow *m_window;

    /// The Vulkan renderer instance
    unique_ptr<VulkanRenderer> m_renderer;

    /// The Vulkan instance of this context.
    vk::Instance m_instance;

    /// The windows surface to present to.
    vk::SurfaceKHR m_surface;

    /// The context logical Vulkan device.
    std::unique_ptr<VulkanHelpers::Device> m_device;

protected:

    /// The list of the extensions required to create an instance
    static std::vector<const char *> requiredInstanceExtensions();

    /// The list of the extensions required to use a certain device
    static inline std::vector<const char *> requiredDeviceExtensions()
    {
        return {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };
    }

    /// The list of validation layers to apply if enabled.
    static inline std::vector<const char*> validationLayers()
    {
           return {
               "VK_LAYER_KHRONOS_validation"
           };
    };

};
