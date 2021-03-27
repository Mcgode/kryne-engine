/**
 * @file
 * @author Max Godefroy
 * @date 17/03/2021.
 */

#pragma once


#include <imgui.h>
#include <kryne-engine/Core/Process.h>


class DearImGuiComponent {

public:

    virtual void renderComponent(Process *process) = 0;

    [[nodiscard]] bool isEnabled() const { return enabled; }

    void setEnabled(bool value) { DearImGuiComponent::enabled = value; }

protected:

    bool enabled = true;

};
