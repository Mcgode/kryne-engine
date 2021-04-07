/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#include "kryne-engine/UI/DearImGuiPerformanceMetrics.hpp"


float DearImGuiPerformanceMetrics::averageFrameTime(vector<float> times) const
{
    float time = 0.f;

    size_t i;
    for (i = 1; i < this->averageSize && i < times.size(); i++)
    {
        if (times[times.size() - i] == 0.)
            break;
        time += times[times.size() - i];
    }
    i--;

    return i ? time / i : 0.f;
}


float DearImGuiPerformanceMetrics::pushTime(const string &name, float value)
{
    auto it = this->timesBuffers.find(name);
    if (it == this->timesBuffers.end())
        it = this->timesBuffers.emplace(name, vector<float>(BUFFER_SIZE)).first;

    auto &times = it->second;
    times.erase(times.begin());
    times.push_back(value);
    return this->averageFrameTime(times);
}


void DearImGuiPerformanceMetrics::renderComponent(Process *process)
{
    if (ImGui::Begin("Performance metrics"))
    {
        auto data = process->getLastFrameTimeData();
        auto frameTime = this->pushTime("Frame times", data.frameTime.count());

        if (ImGui::TreeNode(this, "Average frame time: %.2fms", frameTime * 1000))
        {
            auto &frameTimes = this->timesBuffers.find("Frame times")->second;
            int width = (int) (this->previousPlotWidth - 2 * ImGui::GetStyle().FramePadding.x),
                    size = (int) frameTimes.size();
            ImGui::SetNextItemWidth(-0.1);
            ImGui::PlotLines("##Frame times", frameTimes.data() + std::max(0, size - width),
                             std::min(size, width),
                             0, nullptr,
                             0.f, frameTime * 2,
                             ImVec2(0, 50));
            this->previousPlotWidth = ImGui::GetItemRectSize().x;

            ImGui::TreePop();
        }

        ImGui::Indent();

        ImGui::Text("Average FPS: %.1f", frameTime == 0.f ? 0.f : 1. / frameTime);

        ImGui::Dummy(ImVec2(0, 10));

        if (ImGui::BeginTable("TimingsTable", 2, ImGuiTableFlags_Borders))
        {
            for (const auto &time : data.recordedTimes)
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", time.first.c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2fms", this->pushTime(time.first, time.second.count()) * 1000);
            }
            ImGui::EndTable();
        }

        ImGui::Unindent();

        ImGui::Dummy(ImVec2(0, 10));

        if (ImGui::TreeNode("Options"))
        {
            ImGui::DragInt("Averaging count", reinterpret_cast<int *>(&this->averageSize),
                           1.f, 1, BUFFER_SIZE);
            ImGui::TreePop();
        }
    }

    ImGui::End();
}
