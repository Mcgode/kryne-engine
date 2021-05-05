/**
 * @file
 * @author Max Godefroy
 * @date 07/05/2021.
 */


#include "HelloTriangleApp.hpp"


int main()
{
    HelloTriangleApp app(true);

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}