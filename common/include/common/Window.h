//
// Created by max on 20/04/19.
//

#ifndef INC_3D_DEMOS_WINDOW_H
#define INC_3D_DEMOS_WINDOW_H

#include <vector>
#include <stdarg.h>
#include <stdlib.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"


/**
 * A class representing the GLFW context window
 */
class Window {

public:

    /**
     * A class representing the GLFW context window
     * @param width     The desktop width for the window
     * @param height    The desktop height for the window
     */
    explicit Window(uint32_t width = 1280, uint32_t height = 720);

    ~Window();

private:

    /// The desktop width for the window
    int width;

    /// The desktop height for the window
    int height;

    /// The GLFW desktop window
    GLFWwindow *window;

};


#endif //INC_3D_DEMOS_WINDOW_H
