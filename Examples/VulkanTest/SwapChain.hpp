/**
 * @file
 * @author Max Godefroy
 * @date 04/05/2021.
 */

#pragma once


#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "VertexBuffer.hpp"


class SwapChain {

public:

    SwapChain(const PhysicalDevice &physicalDevice,
              const SurfaceKHR &surface,
              GLFWwindow *window,
              CommandPool *commandPool,
              Device *device,
              const std::vector<VertexBuffer::Vertex> &vertices);

    virtual ~SwapChain();

    bool draw(Semaphore *imageAvailableSemaphore, Semaphore *finishedRenderingSemaphore, Fence *fence,
              const Queue &graphicsQueue, const Queue &presentQueue);

protected:

    void initSwapChain(const PhysicalDevice &physicalDevice, const SurfaceKHR &surface, GLFWwindow *window);

    void setUpImageViews();

    void createRenderPass();

    void createGraphicsPipeline();

    ShaderModule createShaderModule(const std::vector<char> &code);

    void createFramebuffers();

    void createCommandBuffers();

protected:

    struct SwapChainSupportDetails {

        SurfaceCapabilitiesKHR capabilities {};

        std::vector<SurfaceFormatKHR> formats;

        std::vector<PresentModeKHR> presentModes;

    };

    static SwapChainSupportDetails querySwapChainDetails(const PhysicalDevice &physicalDevice, const SurfaceKHR &surface);

    static SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<SurfaceFormatKHR>& availableFormats);

    static PresentModeKHR chooseSwapPresentMode(const std::vector<PresentModeKHR>& availablePresentModes);

    static Extent2D chooseSwapExtent(const SurfaceCapabilitiesKHR& capabilities, GLFWwindow *window);

    static std::vector<char> readFile(const char *filename);

protected:

    Device *device;

    CommandPool *commandPool;

    SwapchainKHR swapchain;

    std::vector<Image> scImages;

    std::vector<ImageView> scImageViews;

    Extent2D scExtend;

    Format scImageFormat {};

    RenderPass renderPass;

    PipelineLayout pipelineLayout;

    Pipeline graphicsPipeline;

    std::vector<Framebuffer> scFramebuffers;

    std::vector<CommandBuffer> commandBuffers;

    std::vector<Fence> imagesInFlight;

    std::unique_ptr<VertexBuffer> vertexBuffer;


};


