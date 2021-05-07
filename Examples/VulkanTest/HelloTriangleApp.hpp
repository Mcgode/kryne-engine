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

    void createSurface();

private:

    static bool checkValidationLayerSupport();

private:

    GLFWwindow *window = nullptr;

    VkInstance instance;

    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDevice device;

    VkSurfaceKHR surface;

};


