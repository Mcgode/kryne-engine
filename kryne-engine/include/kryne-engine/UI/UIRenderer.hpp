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

};


#include <kryne-engine/Core/Process.h>