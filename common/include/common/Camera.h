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

    ~Camera() = default;

};


#endif //INC_3D_DEMOS_CAMERA_H
