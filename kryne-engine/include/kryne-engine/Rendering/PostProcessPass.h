/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#pragma once


#include <iostream>
#include <utility>

#include "LoopRenderer.h"
#include "Framebuffer.hpp"


using namespace std;


class PostProcessPass {


public:

    enum Priority {

        VisualEffect = 0,

        ColorMapping = 1,

        Antialiasing = 2,

        UI = 3,

    };

public:

    [[nodiscard]] Priority getPriority() const { return priority; }

    [[nodiscard]] const string &getName() const { return name; }

protected:

    PostProcessPass(Priority priority, string name);

protected:

    Priority priority;

    string name;


public:

    virtual void processPass(LoopRenderer *renderer, Camera *camera, Framebuffer *readBuffer, Framebuffer *writeBuffer) = 0;


};


