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

        using Utils::FrameTime;

        auto flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
        if (ImGui::BeginTable("TimingsTable", 2, flags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("000.00ms").x);
            ImGui::TableHeadersRow();

            for (auto i = 0; i < FrameTime::Parts::COUNT; i++)
            {
                string name = DearImGuiPerformanceMetrics::getPartString(FrameTime::Parts(i));
                auto d = data.recordedTimes[i];

                ImGui::TableNextRow();

                bool open = false;
                ImGui::TableNextColumn();

                if (d.recordedTimes.empty())
                    ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                else
                    open = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);

                ImGui::TableNextColumn();
                ImGui::Text("%.2fms", this->pushTime(name, d.time.count()) * 1000);

                if (open)
                {
                    ImGui::Indent();

                    for (const auto& p : d.recordedTimes)
                    {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", p.first.c_str());

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%.2fms", this->pushTime(p.first, p.second.count()) * 1000);
                    }

                    ImGui::Unindent();
                    ImGui::TreePop();
                }
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


string DearImGuiPerformanceMetrics::getPartString(Utils::FrameTime::Parts part)
{
    switch (part)
    {
        case Utils::FrameTime::ObjectsScripting:
            return "Scripting";
        case Utils::FrameTime::Rendering:
            return "Rendering";
        case Utils::FrameTime::UI:
            return "UI";
        case Utils::FrameTime::AsyncScripting:
            return "Enqueued tasks";
        case Utils::FrameTime::EventPolling:
            return "Events polling";
        default:
            return "error";
    }
}
