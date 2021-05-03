/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */

#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <algorithm>


class HelloTriangleApp {

public:

    void run();

private:

    void initVulkan();

    void mainLoop();

    void cleanup();

private:

    void createInstance();

    void setupDebugMessenger();

    void pickPhysicalDevice();

    void initLogicalDevice();

    void createSurface();

    void createSwapChain();

    void setUpImageViews();

    void createGraphicsPipeline();

private:

    static bool checkValidationLayerSupport();

private:

    GLFWwindow *window = nullptr;

    VkInstance instance;

    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDevice device;

    VkSurfaceKHR surface;

    VkQueue presentQueue;

    VkSwapchainKHR swapChain;

    std::vector<VkImage> swapChainImages;

    VkFormat swapChainImageFormat;

    VkExtent2D swapChainExtent;

    std::vector<VkImageView> swapChainImageViews;

    VkPipelineLayout pipelineLayout;

};


