/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once


#include "DearIMGUI.h"


/**
 * @brief A UI renderer for the DearIMGUI API, with a customizable defineGUI() function using the functor.
 *
 * @details
 * This class is mainly meant to be used when prototyping a DearGUI interface. It is recommended to make a dedicated
 * class out of the final version.
 */
class DearIMGUIPrototype: public DearIMGUI {

public:

    DearIMGUIPrototype(GLFWwindow *window, const function<void(Process *)> &functor) : DearIMGUI(window), functor(functor)
    {}

protected:

    void defineGUI(Process *process) override
    {
        this->functor(process);
    }

protected:

    function<void(Process *)> functor;

};
