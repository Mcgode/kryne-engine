/**
 * @file
 * @author max
 * @date 10/3/21.
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <common/CommonTypes.hpp>

namespace KryneEngine
{
    struct OpenGlVersion
    {
        u8 m_major: 3;
        u8 m_minor: 5;
    };

    static constexpr OpenGlVersion OpenGlLegacyVersion { 3, 3 };
    static constexpr OpenGlVersion OpenGlBaseVersion { 4, 5 };
    static constexpr OpenGlVersion OpenGlLatestVersion { 4, 6 };

    struct GLContextParameters
    {
        OpenGlVersion m_version = OpenGlBaseVersion;

        s32 m_profile = GLFW_OPENGL_CORE_PROFILE;

        bool m_useDebugLayer = true;
    };
}