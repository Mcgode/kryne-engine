/**
 * @file
 * @author Max Godefroy
 * @date 15/04/2021.
 */

#pragma once


#include <imgui.h>
#include <glm/mat4x4.hpp>

#include <kryne-engine/Textures/CubeTexture.h>
#include <kryne-engine/Rendering/LoopRenderer.h>


namespace KEImGui {

    IMGUI_API void
    CubeMapImageUnwrapped(LoopRenderer *renderer,
                          const shared_ptr<CubeTexture> &cubemap,
                          const ImVec2 &size,
                          float mipLevel = 0,
                          bool dontInvertVertically = false,
                          const ImVec4 &borderColor = ImVec4(0, 0, 0, 0));

}


