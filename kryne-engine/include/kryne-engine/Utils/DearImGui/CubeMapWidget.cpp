/**
 * @file
 * @author Max Godefroy
 * @date 15/04/2021.
 */

#include <kryne-engine/Rendering/LoopRenderer.h>
#include "CubeMapWidget.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui_internal.h"


namespace {

    struct UnwrappedData {

        shared_ptr<CubeTexture> cubeMap;

        glm::ivec2 start;

        glm::ivec2 size;

        float mipLevel;

        LoopRenderer *renderer;

        bool verticalInvert;

        UnwrappedData(const shared_ptr<CubeTexture> &cubeMap,
                      const ImVec2 &start,
                      const ImVec2 &size,
                      float mipLevel,
                      LoopRenderer *renderer,
                      bool verticalInvert)
                          : cubeMap(cubeMap),
                            start(glm::vec2(start.x, start.y)),
                            size(glm::vec2(size.x, size.y)),
                            mipLevel(mipLevel),
                            renderer(renderer),
                            verticalInvert(verticalInvert)
        {}

    };


    ShaderMaterial &unwrappedShader()
    {
        static ShaderMaterial mat(make_unique<Shader>("Engine/Utils/UnwrappedCubeMap"));
        return mat;
    }


    void drawUnwrapped(const ImDrawList *_, const ImDrawCmd *cmd)
    {
        auto data = (UnwrappedData *)(cmd->UserCallbackData);

        unwrappedShader().setUniform("cubeMap", data->cubeMap);
        unwrappedShader().setUniform("mipLevel", data->mipLevel);
        unwrappedShader().setUniform("invertVertical", data->verticalInvert);
        data->renderer->quadRender(&unwrappedShader(), data->start, data->size);

        delete data;
    }

}


void KEImGui::CubeMapImageUnwrapped(LoopRenderer *renderer,
                                    const shared_ptr<CubeTexture> &cubemap,
                                    const ImVec2 &size,
                                    float mipLevel,
                                    bool dontInvertVertically,
                                    const ImVec4 &borderColor)
{
    using namespace ImGui;

    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    if (borderColor.w > 0.0f)
        bb.Max += ImVec2(2, 2);
    ItemSize(bb);

    if (!ItemAdd(bb, 0))
        return;

    ImVec2 actualSize = size;
    ImVec2 actualPos = bb.Min;

    if (borderColor.w != 0.f)
    {
        actualSize -= ImVec2(2, 2);
        actualPos += ImVec2(1, 1);
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(borderColor), 0.f);
    }

    window->DrawList->AddCallback(drawUnwrapped, new UnwrappedData(cubemap,
                                                                              actualPos, actualSize,
                                                                              mipLevel, renderer,
                                                                              !dontInvertVertically));
    window->DrawList->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
    window->DrawList->AddDrawCmd();
}
