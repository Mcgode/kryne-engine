//
// Created by max on 21/05/19.
//

#include "common/Camera/TrackballCamera.h"

TrackballCamera::TrackballCamera(glm::vec3 centerPoint, float zoomDistance)
{
    this->centerPosition = glm::vec3(centerPoint);
    this->zoomDistance = zoomDistance;
    this->rotation = glm::quat();
}


glm::vec3 TrackballCamera::getCurrentPosition()
{
    glm::vec3 rotationPosition = glm::mat3_cast(rotation) * glm::vec3(0, 0, -zoomDistance);
    return glm::vec3() + this->centerPosition;
}


glm::mat4 TrackballCamera::getViewMatrix()
{
    return glm::translate(
            glm::mat4_cast(rotation) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -zoomDistance)),
            -centerPosition
            );
}
