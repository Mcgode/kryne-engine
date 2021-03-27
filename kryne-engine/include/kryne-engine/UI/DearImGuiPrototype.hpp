/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once


#include "DearImGuiComponent.hpp"


/**
 * @brief A UI renderer for the DearIMGUI API, with a customizable defineGUI() function using the functor.
 *
 * @details
 * This class is mainly meant to be used when prototyping a DearGUI interface. It is recommended to make a dedicated
 * class out of the final version.
 */
class DearImGuiPrototype: public DearImGuiComponent {

public:

    explicit DearImGuiPrototype(const function<void(Process *)> &functor) : functor(functor)
    {}

    void renderComponent(Process *process) override
    {
        this->functor(process);
    }

protected:

    function<void(Process *)> functor;

};
