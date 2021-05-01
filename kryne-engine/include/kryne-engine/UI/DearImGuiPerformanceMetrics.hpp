/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#pragma once


#include "DearImGuiComponent.hpp"


class DearImGuiPerformanceMetrics : public DearImGuiComponent {

public:

    void renderComponent(Process *process) override;

protected:

    [[nodiscard]] float averageFrameTime(vector<float> times) const;

    float pushTime(const string &name, float value);

    static string getPartString(Utils::FrameTime::Parts part);

protected:

    const static size_t BUFFER_SIZE = 1000;

    size_t averageSize = 30;

    unordered_map<string, vector<float>> timesBuffers;

    /// The width of the plot widget in the previous frame
    float previousPlotWidth;

};


