/**
 * @file
 * @author Max Godefroy
 * @date 04/05/2021.
 */

#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>


namespace VulkanHelpers {

    void assertResult(vk::Result result)
    {
#ifndef NDEBUG

        if (result != vk::Result::eSuccess)
            throw std::runtime_error(vk::to_string(result));

#endif
    }

    void assertResult(vk::Result result, const std::string &errorMessage)
    {
#ifndef NDEBUG

        if (result != vk::Result::eSuccess)
            throw std::runtime_error(errorMessage + ": " + vk::to_string(result));

#endif
    }

}
