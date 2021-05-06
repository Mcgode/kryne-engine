/**
 * @file
 * @author Max Godefroy
 * @date 04/05/2021.
 */

#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>


namespace VulkanHelpers {

    void assertSuccess(vk::Result result);

    void assertSuccess(vk::Result result, const std::string &errorMessage);

    template <typename T>
    T assertSuccess(vk::ResultValue<T> result)
    {
#ifndef NDEBUG

        if (result.result != vk::Result::eSuccess)
            throw std::runtime_error(vk::to_string(result.result));

#endif

        return std::move(result.value);
    }

    template <typename T>
    T assertSuccess(vk::ResultValue<T> result, const std::string &errorMessage)
    {
#ifndef NDEBUG

        if (result.result != vk::Result::eSuccess)
            throw std::runtime_error(errorMessage + ": " + vk::to_string(result.result));

#endif

        return std::move(result.value);
    }

}
