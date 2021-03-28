/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#pragma once


#include "DearImGuiComponent.hpp"


class DearImGuiPerformanceMetrics : public DearImGuiComponent {

public:

    DearImGuiPerformanceMetrics(): frameTimes(vector<float>(1000)) {}

    void renderComponent(Process *process) override;

protected:

    float averageFrameTime();

protected:

    size_t averageSize = 30;

    vector<float> frameTimes;

};


