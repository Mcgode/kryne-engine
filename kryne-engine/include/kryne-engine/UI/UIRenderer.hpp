/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once


class Process;


class UIRenderer {

public:

    virtual void render(Process *process) = 0;

    [[nodiscard]] bool isEnabled() const { return enabled; }

    void setEnabled(bool value) { UIRenderer::enabled = value; }

protected:

    bool enabled = true;

};


#include <kryne-engine/Core/Process.h>