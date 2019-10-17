//
// Created by max on 26/07/2019.
//

#include "kryne-engine/Camera/FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(float speed, float drag)
{
    this->position = glm::vec3(0, 0, 3);
    this->pitch = 0;
    this->yaw = 0;
    this->speed = speed;
    this->drag = drag;

    this->lastFrameTime = std::chrono::system_clock::now();
}

glm::vec3 FirstPersonCamera::getCurrentPosition()
{
    return this->position;
}

glm::mat4 FirstPersonCamera::getViewMatrix()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    auto cameraFront = glm::normalize(front);
    return glm::lookAt(position, position + cameraFront, glm::vec3(0, 1, 0));
}

void FirstPersonCamera::onMouseMovementInput(Window *window, double xInput, double yInput)
{
    window->setMouseCursor(GLFW_CURSOR_DISABLED);

    auto dv = glm::vec2();

    if (!isnan(lastMouseXPosition))
        dv = glm::vec2(xInput - lastMouseXPosition, yInput - lastMouseYPosition);

    yaw += dv.x * drag;
    pitch = max(-89.0, min(89.0, pitch - dv.y * drag));

    lastMouseXPosition = xInput;
    lastMouseYPosition = yInput;
}


void FirstPersonCamera::frameUpdate(Window *window)
{
    auto dv = glm::vec3(0.0);

    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = now - this->lastFrameTime;
    this->lastFrameTime = now;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    auto cameraFront = glm::normalize(front);

    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_UP) == GLFW_PRESS)
        dv += cameraFront;
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
        dv -= cameraFront;
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        dv += glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0)));
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
        dv -= glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0)));
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        dv += glm::vec3(0, 1, 0);
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        dv -= glm::vec3(0, 1, 0);

    auto f = glm::vec3(speed * elapsed_seconds.count());
    dv = glm::length(dv) ? glm::normalize(dv) * f : dv;

    position += dv;
}
