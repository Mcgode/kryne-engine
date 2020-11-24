/**
 * @file
 * @author Max Godefroy
 * @date 20/04/19
 */

#ifndef INC_3D_DEMOS_WINDOW_H
#define INC_3D_DEMOS_WINDOW_H

#include <vector>
#include <unordered_set>
#include <cstdarg>
#include <cstdlib>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "AdditionalParameters.h"

class OldCamera;
#include <kryne-engine/Camera/OldCamera.h>


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

    int getWidth() const;

    int getHeight() const;

    GLFWwindow *getGlfwWindow() const;

    static void keyCallback(GLFWwindow *_w, int key, int _scan_code, int action, int _mods);

    static void scrollCallback(GLFWwindow *_w, double _, double scroll);

    static void mouseMovementCallback(GLFWwindow *_w, double xPos, double yPos);

    void setMouseCursor(int value);

    void setCurrentCamera(OldCamera *camera);

    ~Window();

private:

    /// The desktop width for the window
    int width;

    /// The desktop height for the window
    int height;

    /// The GLFW desktop window
    GLFWwindow *window;

    /// The list of currently pressed keys
    std::unordered_set<int> keysPressed;

};


#endif //INC_3D_DEMOS_WINDOW_H
