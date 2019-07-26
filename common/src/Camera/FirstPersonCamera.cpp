//
// Created by max on 26/07/2019.
//

#include "common/Camera/FirstPersonCamera.h"

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
    return glm::translate(glm::yawPitchRoll(-(float) yaw, -(float) pitch, 0.f), -position);
}

void FirstPersonCamera::onMouseMovementInput(Window *window, double xInput, double yInput)
{
    auto dv = glm::vec2(xInput - lastMouseXPosition, yInput - lastMouseYPosition);

    yaw -= dv.x * drag * 2 * glm::pi<double>();
    pitch = max(-glm::pi<double>() / 2., min(glm::pi<double>() / 2., pitch - dv.y * drag * 2 * glm::pi<double>()));

    lastMouseXPosition += dv.x;
    lastMouseYPosition += dv.y;
}


void FirstPersonCamera::frameUpdate(Window *window)
{
    auto dv = glm::vec3(0.0);

    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = now - this->lastFrameTime;
    this->lastFrameTime = now;

    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        dv.x += 1.0f;
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
        dv.x -= 1.0f;
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        dv.y += 1.0f;
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        dv.y -= 1.0f;
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
        dv.z += 1.0f;
    if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_UP) == GLFW_PRESS)
        dv.z -= 1.0f;

    auto f = glm::vec3(speed * elapsed_seconds.count());
    dv = glm::length(dv) ? glm::normalize(dv) * f : dv;

    position += glm::mat3(glm::yawPitchRoll((float) yaw, (float) pitch, 0.f)) * dv;
}
