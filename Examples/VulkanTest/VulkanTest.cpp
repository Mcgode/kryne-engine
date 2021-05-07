/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */


#include "HelloTriangleApp.hpp"


int main()
{
    try
    {
        HelloTriangleApp app;
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}