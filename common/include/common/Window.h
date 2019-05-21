//
// Created by max on 20/04/19.
//

#ifndef INC_3D_DEMOS_WINDOW_H
#define INC_3D_DEMOS_WINDOW_H

#include <vector>
#include <unordered_set>
#include <stdarg.h>
#include <stdlib.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Types.h"


class Scene;


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

    void runLoop(Scene *scene, AdditionalParams_t *params = nullptr);

    static void keyCallback(GLFWwindow *_w, int key, int _scan_code, int action, int _mods);

    static void scrollCallback(GLFWwindow *_w, double _, double scroll);

    static void mouseMovementCallback(GLFWwindow *_w, double xPos, double yPos);

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

    Scene *currentScene;

};


#endif //INC_3D_DEMOS_WINDOW_H
