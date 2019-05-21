//
// Created by max on 20/04/19.
//

#include "common/Camera/BasicCamera.h"

BasicCamera::BasicCamera(glm::vec3 initial_position, glm::mat4 initial_rotation)
{
    position = initial_position;
    rotation = initial_rotation;
}


BasicCamera::BasicCamera(glm::vec3 camera_position, glm::vec3 target_position, glm::vec3 up_vector)
{
    position = camera_position;
    rotation = glm::lookAt(camera_position, target_position, up_vector);
}


glm::mat4 BasicCamera::getViewMatrix()
{
    return glm::translate(rotation, -position);
}


glm::vec3 BasicCamera::getCurrentPosition()
{
    return glm::vec3(position);
}
