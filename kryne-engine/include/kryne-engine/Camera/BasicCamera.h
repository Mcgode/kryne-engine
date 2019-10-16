//
// Created by max on 20/04/19.
//

#ifndef INC_3D_DEMOS_BASICCAMERA_H
#define INC_3D_DEMOS_BASICCAMERA_H

#include <vector>
#include <stdarg.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"


/**
 * A class representing a basic camera
 */
class BasicCamera : public Camera {

public:

    /**
     * A class representing a basic camera
     * @param initial_position The initial position of the camera
     * @param initial_rotation The initial rotation matrix of the camera
     */
    explicit BasicCamera(glm::vec3 initial_position, glm::mat4 initial_rotation = glm::mat4());

    /**
     * A class representing a basic camera
     * @param camera_position The position of the camera
     * @param target_position The position targeted by the camera
     * @param up_vector The up vector for the lookAt function
     */
    BasicCamera(glm::vec3 camera_position, glm::vec3 target_position, glm::vec3 up_vector = glm::vec3(0, 1, 0));

    glm::mat4 getViewMatrix() override;

    glm::vec3 getCurrentPosition() override;

private:

    /// The position of the camera
    glm::vec3 position;

    /// The current rotation matrix for this camera
    glm::mat4 rotation;
};


#endif //INC_3D_DEMOS_BASICCAMERA_H
