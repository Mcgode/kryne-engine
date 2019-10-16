//
// Created by max on 20/04/19.
//

#ifndef INC_3D_DEMOS_CAMERA_H
#define INC_3D_DEMOS_CAMERA_H

#include <vector>
#include <stdarg.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/Window.h>


/**
 * An abstract class for representing a camera
 */
class Camera {

public:

    /**
     * Retrieves the view matrix for this camera
     * @return The view matrix for this camera.
     */
    virtual glm::mat4 getViewMatrix() = 0;

    /**
     * Retrieves the current position of the camera
     * @return The current position of the camera
     */
    virtual glm::vec3 getCurrentPosition() = 0;

    /**
     * Handles a key press input from the user
     * @param window    The render window object
     * @param key       The key pressed by the user
     */
    virtual void onKeyInput(Window *window, GLint key) = 0;

    /**
     * Handles a scroll input from the user
     * @param window    The render window object
     * @param value     The scroll intensity from the user
     */
    virtual void onScrollInput(Window *window, float value) = 0;

    /**
     * Handles mouse movements from the user
     * @param window    The render window object
     * @param xInput    The new x position of the mouse
     * @param yInput    The new y position of the mouse
     */
    virtual void onMouseMovementInput(Window *window, double xInput, double yInput) = 0;

    virtual void frameUpdate(Window *window) = 0;

    virtual ~Camera() = default;

};


#endif //INC_3D_DEMOS_CAMERA_H
