/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#include "kryne-engine/UI/DearImGuiPerformanceMetrics.hpp"

void DearImGuiPerformanceMetrics::renderComponent(Process *process)
{
    ImGui::Begin("Performance metrics");

    auto data = process->getLastFrameTimeData();
    this->frameTimes.erase(this->frameTimes.begin());
    this->frameTimes.push_back(data.frameTime.count());
    auto frameTime = averageFrameTime();

    ImGui::PlotLines("Frame times", this->frameTimes.data(), this->frameTimes.size(),
                     0, nullptr,
                     0.f, FLT_MAX,
                     ImVec2(0, 50.f));
    ImGui::Text("Average frame time: %.2fms", frameTime * 1000);
    ImGui::Text("Average FPS: %.1f", frameTime == 0.f ? 0.f : 1. / frameTime);

    if (ImGui::BeginTable("TimingsTable", 2))
    {
        for (const auto &time : data.recordedTimes)
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", time.first.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.2fms", time.second.count() * 1000);
        }
        ImGui::EndTable();
    }

    if (ImGui::TreeNode("Options"))
    {
        ImGui::DragInt("Averaging count", reinterpret_cast<int *>(&this->averageSize),
                       1.f, 1, this->frameTimes.size());
        ImGui::TreePop();
    }

    ImGui::End();
}

float DearImGuiPerformanceMetrics::averageFrameTime()
{
    float time = 0.f;

    size_t i;
    for (i = 1; i < this->averageSize && i < this->frameTimes.size(); i++)
    {
        if (this->frameTimes[i] == 0.)
        {
            i--;
            break;
        }
        time += this->frameTimes[i];
    }

    return i ? time / i : 0.f;
}
