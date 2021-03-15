/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once


#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "UIRenderer.hpp"


class DearIMGUI: public UIRenderer {

public:

    explicit DearIMGUI(GLFWwindow *window);

    void render(Process *process) override;

    virtual ~DearIMGUI();

protected:

    virtual void defineGUI(Process *process);

};

