//
// Created by max on 21/05/19.
//

#include "kryne-engine/Camera/TrackballCamera.h"

TrackballCamera::TrackballCamera(glm::vec3 centerPoint, float zoomDistance)
{
    this->centerPosition = glm::vec3(centerPoint);
    this->zoomDistance = zoomDistance;
    this->phi = 0;
    this->theta = 0;
}


glm::vec3 TrackballCamera::getCurrentPosition()
{
    glm::vec3 rotationPosition = glm::vec3(0, 0, -zoomDistance) * glm::mat3(getRotation());
    return rotationPosition + this->centerPosition;
}


glm::mat4 TrackballCamera::getViewMatrix()
{
    return glm::translate(
            glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -zoomDistance)) * getRotation(),
            -centerPosition
            );
}


void TrackballCamera::onScrollInput(Window *window, float value)
{
    zoomDistance = fmax(0.01, zoomDistance * (1.0 - 50. * value / window->getHeight()));
}


void TrackballCamera::onMouseMovementInput(Window *window, double xInput, double yInput)
{
    if (glfwGetMouseButton(window->getGlfwWindow(), GLFW_MOUSE_BUTTON_1)) {
        if (!wasMoving) {
            wasMoving = true;
            lastMouseXPosition = xInput;
            lastMouseYPosition = yInput;
        }

#define DRAG_SPEED 0.05
        float pitch = (xInput - lastMouseXPosition) * DRAG_SPEED / window->getWidth();
        float yaw = (yInput - lastMouseYPosition) * DRAG_SPEED / window->getHeight();

        phi = fmax(-glm::pi<float>(), fmin(glm::pi<float>(), phi + yaw));
        theta += pitch;
    } else if (glfwGetMouseButton(window->getGlfwWindow(), GLFW_MOUSE_BUTTON_2)) {
        if (!wasMoving) {
            wasMoving = true;
            lastMouseXPosition = xInput;
            lastMouseYPosition = yInput;
        }
        glm::mat3 rot = glm::mat3(getRotation());

#define PAN_SPEED 0.01
        glm::vec3 dx = -rot * glm::vec3(PAN_SPEED * zoomDistance * (xInput - lastMouseXPosition) / window->getWidth(), 0, 0);
        glm::vec3 dy = rot * glm::vec3(0, PAN_SPEED * zoomDistance * (yInput - lastMouseYPosition) / window->getHeight(), 0);

//        std::cout << "dx : (" << dx[0] << ", " << dx[1] << ", " << dx[2] << ")" << std::endl;
//        std::cout << "dy : (" << dy[0] << ", " << dy[1] << ", " << dy[2] << ")" << std::endl;

        centerPosition = centerPosition + dx + dy;
    } else {
        wasMoving = false;
    }
}


glm::mat4 TrackballCamera::getRotation() {
    return glm::rotate(glm::rotate(glm::mat4(1), theta, glm::vec3(0, 1, 0)), phi, glm::vec3(1, 0, 0));
}
