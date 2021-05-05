/**
 * @file
 * @author Max Godefroy
 * @date 04/05/2021.
 */

#include <fstream>
#include "Utils.hpp"
#include "Queue.hpp"

#include "SwapChain.hpp"


using VulkanHelpers::assertSuccess;


SwapChain::SwapChain(const PhysicalDevice &physicalDevice, const SurfaceKHR &surface, GLFWwindow *window,
                     CommandPool *commandPool, Device *device, const std::vector<VertexBuffer::Vertex> &vertices)
{
    this->commandPool = commandPool;
    this->device = device;

    this->initSwapChain(physicalDevice, surface, window);
    this->setUpImageViews();
    this->createRenderPass();

    this->vertexBuffer = std::make_unique<VertexBuffer>(physicalDevice, this->device, vertices);
    this->createGraphicsPipeline();

    this->createFramebuffers();
    this->createCommandBuffers();

    this->imagesInFlight.resize(this->scImageViews.size(), {});
}


SwapChain::~SwapChain()
{
    this->device->freeCommandBuffers(*this->commandPool, this->commandBuffers);

    for (const auto &fb : this->scFramebuffers)
        this->device->destroyFramebuffer(fb);

    this->device->destroyPipeline(this->graphicsPipeline);
    this->device->destroyPipelineLayout(this->pipelineLayout);

    this->device->destroyRenderPass(this->renderPass);

    for (const auto &iv : this->scImageViews)
        this->device->destroyImageView(iv);

    this->device->destroy(this->swapchain);
}


SwapChain::SwapChainSupportDetails SwapChain::querySwapChainDetails(const PhysicalDevice &physicalDevice, const SurfaceKHR &surface)
{
    SwapChainSupportDetails details;

    details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    details.formats = physicalDevice.getSurfaceFormatsKHR(surface);
    details.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);

    return details;
}


SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<SurfaceFormatKHR> &availableFormats)
{
    for (const auto &format : availableFormats)
    {
        if (format.format == Format::eB8G8R8A8Srgb && format.colorSpace == ColorSpaceKHR::eSrgbNonlinear)
            return format;
    }
    return availableFormats[0];
}


PresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<PresentModeKHR> &availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == PresentModeKHR::eMailbox)
            return availablePresentMode;
    }

    return PresentModeKHR::eFifo;
}


Extent2D SwapChain::chooseSwapExtent(const SurfaceCapabilitiesKHR &capabilities, GLFWwindow *window)
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


std::vector<char> SwapChain::readFile(const char *filename)
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


void SwapChain::initSwapChain(const PhysicalDevice &physicalDevice, const SurfaceKHR &surface, GLFWwindow *window)
{
    auto details = querySwapChainDetails(physicalDevice, surface);

    auto surfaceFormat = chooseSwapSurfaceFormat(details.formats);
    auto presentMode = chooseSwapPresentMode(details.presentModes);
    auto extent = chooseSwapExtent(details.capabilities, window);

    uint32_t imageCount = details.capabilities.minImageCount + 1;

    if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
        imageCount = details.capabilities.maxImageCount;

    const auto indices = VulkanHelpers::findQueueFamilies(physicalDevice, surface);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    bool concurrent = indices.graphicsFamily != indices.presentFamily;

    SwapchainCreateInfoKHR createInfo({}, surface,
                                      imageCount,
                                      surfaceFormat.format, surfaceFormat.colorSpace,
                                      extent,
                                      1, ImageUsageFlagBits::eColorAttachment,
                                      concurrent ? SharingMode::eConcurrent : SharingMode::eExclusive,
                                      concurrent ? 2 : 0,
                                      concurrent ? queueFamilyIndices : nullptr,
                                      details.capabilities.currentTransform,
                                      CompositeAlphaFlagBitsKHR::eOpaque,
                                      presentMode,
                                      true,
                                      {});

    assertSuccess(this->device->createSwapchainKHR(&createInfo, nullptr, &this->swapchain),
                  "Unable to create swapchain");

    this->scImages = this->device->getSwapchainImagesKHR(this->swapchain);

    this->scExtend = extent;
    this->scImageFormat = surfaceFormat.format;
}


void SwapChain::setUpImageViews()
{
    this->scImageViews.resize(this->scImages.size());

    for (auto i = 0; i < this->scImageViews.size(); i++)
    {
        ImageViewCreateInfo createInfo({}, this->scImages[i],
                                       ImageViewType::e2D,
                                       this->scImageFormat,
                                       {},
                                       { ImageAspectFlagBits::eColor, 0, 1, 0, 1 });

        assertSuccess(this->device->createImageView(&createInfo, nullptr, &this->scImageViews[i]),
                      "Unable to initialize image view");
    }
}


void SwapChain::createRenderPass()
{
    AttachmentDescription colorAttachment({}, this->scImageFormat, SampleCountFlagBits::e1,
                                          AttachmentLoadOp::eClear, AttachmentStoreOp::eStore,
                                          AttachmentLoadOp::eDontCare, AttachmentStoreOp::eDontCare,
                                          ImageLayout::eUndefined, ImageLayout::ePresentSrcKHR);

    AttachmentReference colorRef(0, ImageLayout::eColorAttachmentOptimal);

    SubpassDescription spDesc({}, PipelineBindPoint::eGraphics,
                              0, nullptr,
                              1, &colorRef);

    SubpassDependency spDep(VK_SUBPASS_EXTERNAL, 0,
                            PipelineStageFlagBits::eColorAttachmentOutput, PipelineStageFlagBits::eColorAttachmentOutput,
                            AccessFlagBits::eNoneKHR, AccessFlagBits::eColorAttachmentWrite);

    RenderPassCreateInfo rpcInfo({},
                                 1, &colorAttachment,
                                 1, &spDesc,
                                 1, &spDep);

    assertSuccess(this->device->createRenderPass(&rpcInfo, nullptr, &this->renderPass),
                  "Unable to create render pass");
}


ShaderModule SwapChain::createShaderModule(const std::vector<char> &code)
{
    ShaderModuleCreateInfo createInfo({}, code.size(), reinterpret_cast<const uint32_t*>(code.data()));

    ShaderModule shaderModule;
    assertSuccess(this->device->createShaderModule(&createInfo, nullptr, &shaderModule),
                  "Unable to create shader module");

    return shaderModule;
}


void SwapChain::createGraphicsPipeline()
{
    auto vertShader = readFile("Resources/Shaders/Vulkan/TriangleV.spv");
    auto fragShader = readFile("Resources/Shaders/Vulkan/TriangleF.spv");

    auto vModule = this->createShaderModule(vertShader);
    auto fModule = this->createShaderModule(fragShader);

    PipelineShaderStageCreateInfo vsInfo({}, ShaderStageFlagBits::eVertex, vModule, "main");
    PipelineShaderStageCreateInfo fsInfo({}, ShaderStageFlagBits::eFragment, fModule, "main");

    PipelineShaderStageCreateInfo shaderStages[] = { vsInfo, fsInfo };

    auto vcInfo = this->vertexBuffer->getPipelineVertexCreationInfo();
    PipelineInputAssemblyStateCreateInfo asmInfo({}, PrimitiveTopology::eTriangleList, false);

    Viewport vp(0, 0, this->scExtend.width, this->scExtend.height, 0, 1);

    Rect2D scissor({0, 0}, this->scExtend);

    PipelineViewportStateCreateInfo vpInfo({}, 1, &vp, 1, &scissor);

    PipelineRasterizationStateCreateInfo rastInfo({}, false, false,
                                                  PolygonMode::eFill,
                                                  CullModeFlagBits::eBack, FrontFace::eClockwise,
                                                  false, 0, 0, 0,
                                                  1);

    PipelineMultisampleStateCreateInfo msInfo({}, SampleCountFlagBits::e1, VK_FALSE);

    PipelineColorBlendAttachmentState cbState(false);
    // Important, because it's initialized at 0 !!!
    cbState.colorWriteMask = ColorComponentFlagBits::eR | ColorComponentFlagBits::eG | ColorComponentFlagBits::eB | ColorComponentFlagBits::eA;

    PipelineColorBlendStateCreateInfo cbInfo({}, false, LogicOp::eClear, 1, &cbState);

    PipelineLayoutCreateInfo layoutInfo;
    this->pipelineLayout = this->device->createPipelineLayout(layoutInfo);

    GraphicsPipelineCreateInfo creationInfo({}, 2, shaderStages, &vcInfo, &asmInfo,  nullptr,
                                            &vpInfo, &rastInfo, &msInfo, nullptr, &cbInfo, nullptr,
                                            this->pipelineLayout, this->renderPass, 0,
                                            {}, -1);

    this->graphicsPipeline = assertSuccess(this->device->createGraphicsPipeline({}, creationInfo),
                                           "Unable to create pipeline");
}


void SwapChain::createFramebuffers()
{
    this->scFramebuffers.resize(this->scImageViews.size());

    for (size_t i = 0; i < this->scImageViews.size(); i++)
    {
        ImageView attachments[] = { this->scImageViews[i] };

        FramebufferCreateInfo framebufferInfo( {}, this->renderPass, 1, attachments,
                                               this->scExtend.width, this->scExtend.height, 1 );

        this->scFramebuffers[i] = this->device->createFramebuffer(framebufferInfo);
    }
}


void SwapChain::createCommandBuffers()
{
    CommandBufferAllocateInfo allocInfo(*this->commandPool, CommandBufferLevel::ePrimary,
                                        this->scFramebuffers.size());

    this->commandBuffers = this->device->allocateCommandBuffers(allocInfo);

    for (auto i = 0; i < this->commandBuffers.size(); i++)
    {
        CommandBufferBeginInfo beginInfo;

        this->commandBuffers[i].begin(beginInfo);

        std::array<float, 4> ccArray = { 0, 0, 0, 1};
        ClearValue clearColor(ccArray);

        RenderPassBeginInfo rpInfo(this->renderPass, this->scFramebuffers[i],
                                   { {0, 0}, this->scExtend },
                                   1, &clearColor);

        this->commandBuffers[i].beginRenderPass(rpInfo, SubpassContents::eInline);
        this->commandBuffers[i].bindPipeline(PipelineBindPoint::eGraphics, this->graphicsPipeline);
        this->commandBuffers[i].draw(3, 1, 0, 0);
        this->commandBuffers[i].endRenderPass();

        this->commandBuffers[i].end();
    }
}


bool SwapChain::draw(Semaphore *imageAvailableSemaphore, Semaphore *finishedRenderingSemaphore, Fence *fence,
                     const Queue &graphicsQueue, const Queue &presentQueue)
{
    assertSuccess(this->device->waitForFences(1, fence, VK_TRUE, UINT64_MAX));

    auto acquireResult = this->device->acquireNextImageKHR(this->swapchain, UINT64_MAX, *imageAvailableSemaphore);

    switch (acquireResult.result)
    {
        case Result::eErrorOutOfDateKHR:
            return false;
        case Result::eSuccess:
        case Result::eSuboptimalKHR:
            break;
        default:
            throw std::runtime_error("Unable to swap images: " + to_string(acquireResult.result));
    }

    auto index = acquireResult.value;

    if (this->imagesInFlight[index])
        assertSuccess(this->device->waitForFences(1, &this->imagesInFlight[index], VK_TRUE, UINT64_MAX));
    this->imagesInFlight[index] = *fence;

    PipelineStageFlags flags[] = { PipelineStageFlagBits::eColorAttachmentOutput };
    SubmitInfo info(1, imageAvailableSemaphore, flags,
                    1, &this->commandBuffers[index],
                    1, finishedRenderingSemaphore);

    assertSuccess(this->device->resetFences(1, fence));

    assertSuccess(graphicsQueue.submit(1, &info, *fence));

    PresentInfoKHR presentInfo(1, finishedRenderingSemaphore,
                               1, &this->swapchain, &index, nullptr);

    const auto presentResult = presentQueue.presentKHR(&presentInfo);
    if (presentResult == Result::eErrorOutOfDateKHR || presentResult == Result::eSuboptimalKHR)
        return false;
    assertSuccess(presentResult);

    return true;
}
