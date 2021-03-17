/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#include "kryne-engine/UI/DearImGui.h"

DearImGui::DearImGui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}


DearImGui::~DearImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void DearImGui::render(Process *process)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (const auto &component : this->components)
        component->renderComponent(process);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    auto &io = ImGui::GetIO();
    process->getPlayerInput()->setExternallyCaptured(io.WantCaptureMouse, io.WantCaptureKeyboard);
}
