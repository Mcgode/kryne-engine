/**
 * @file
 * @author Max Godefroy
 * @date 16/05/2021.
 */

#pragma once

#ifdef KRYNE_ENGINE_VULKAN


#include "../Core/GraphicContext/GraphicContext.h"


class GLFWwindow;
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

    GLFWwindow* m_window;

    unique_ptr<VulkanRenderer> m_renderer;

};

#endif
