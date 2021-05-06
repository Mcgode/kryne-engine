/**
 * @file
 * @author Max Godefroy
 * @date 06/05/2021.
 */


#include "Utils.hpp"

void VulkanHelpers::assertSuccess(vk::Result result)
{
#ifndef NDEBUG

    if (result != vk::Result::eSuccess)
        throw std::runtime_error(vk::to_string(result));

#endif
}

void VulkanHelpers::assertSuccess(vk::Result result, const std::string &errorMessage)
{
#ifndef NDEBUG

    if (result != vk::Result::eSuccess)
        throw std::runtime_error(errorMessage + ": " + vk::to_string(result));

#endif
}
