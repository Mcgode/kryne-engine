/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once


#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "UIRenderer.hpp"
#include "DearImGuiComponent.hpp"


class DearImGui: public UIRenderer {

public:

    explicit DearImGui(GLFWwindow *window);

    void render(Process *process) override;

    [[nodiscard]] Utils::UniquePtrVector<DearImGuiComponent> &getComponents() { return components; }

    virtual ~DearImGui();

protected:

    Utils::UniquePtrVector<DearImGuiComponent> components;

};

