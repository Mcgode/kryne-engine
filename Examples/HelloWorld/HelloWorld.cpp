/**
 * @file
 * @author max
 * @date 10/30/21.
 */

#include <ke/Application.hpp>
#include <ke/Window.hpp>
#include <ke/graphics/opengl/GlContextParameters.hpp>
#include <iostream>


int main()
{
    using namespace KryneEngine;

    WindowDescription wDesc;
    GLContextParameters glParams;

    try
    {
        Application::GetApplication().CreateWindow(wDesc, &glParams);
    }
    catch (std::exception& e)
    {
        std::cerr << "Encountered exception!" << std::endl << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}