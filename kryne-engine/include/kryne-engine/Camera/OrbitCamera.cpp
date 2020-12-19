/**
 * @file
 * @author Max Godefroy
 * @date 16/12/2020.
 */

#include "OrbitCamera.h"

#include <utility>

OrbitCamera::OrbitCamera(unique_ptr<ProjectionData> projectionData, weak_ptr<PlayerInput> playerInput) :
    Camera(move(projectionData)),
    playerInput(std::move(playerInput))
{
    this->position = glm::vec3(0, 0, 5);
}


void OrbitCamera::update(bool force)
{
    const auto input = this->playerInput.lock();

    if (input != nullptr)
    {
        const auto dir = this->position - this->centerPosition;
        this->distance = length(dir);

        const auto movement = input->getCursorMovement();
        auto newAngle = this->angle;

        if (input->isKeyDown(GLFW_MOUSE_BUTTON_1)) {
            newAngle.x += -movement.x * this->radiansPerMousePixel / glm::max(cos(angle.y), 0.01f);
            newAngle.y += movement.y * this->radiansPerMousePixel;
            newAngle.y = glm::min(glm::max(newAngle.y, -this->maxPhi), this->maxPhi);
        }

        if (this->angle != newAngle)
        {
            this->angle = newAngle;
            this->updatePosition(newAngle, this->distance);
        }

    }

    Camera::update(force);
}

void OrbitCamera::setPosition(const vec3 &pos)
{
    auto p = pos - this->centerPosition;
    this->distance = length(p);
    p /= this->distance;

    const float phi = asin(p.y);
    const float theta = p.z != 0 ? atan2(p.x, p.z) : (p.x >= 0 ? 0 : pi<float>());

    this->angle = vec2(theta, phi);

    this->updatePosition(this->angle, this->distance);
}


void OrbitCamera::updatePosition(const vec2 &newAngle, float dist)
{
    float cosPhi = cos(newAngle.y);
    auto pos = vec3(cosPhi * sin(newAngle.x), sin(newAngle.y), cosPhi * cos(newAngle.x));
    pos *= dist;
    Object3D::setPosition(pos + this->centerPosition);
    this->lookAt(this->centerPosition);
}
