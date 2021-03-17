/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#include "kryne-engine/UI/DearIMGUI.h"

DearIMGUI::DearIMGUI(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}


DearIMGUI::~DearIMGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void DearIMGUI::render(Process *process)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    this->defineGUI(process);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    auto &io = ImGui::GetIO();
    process->getPlayerInput()->setExternallyCaptured(io.WantCaptureMouse, io.WantCaptureKeyboard);
}


void DearIMGUI::defineGUI(Process *process)
{
    ImGui::Begin("Demo window");
    ImGui::Button("Hello!");
    ImGui::End();

    ImGui::ShowDemoWindow();
}
