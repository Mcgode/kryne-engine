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
            newAngle.x += -movement.x * this->radiansPerMousePixel;
            newAngle.y += movement.y * this->radiansPerMousePixel;
            newAngle.y = glm::min(glm::max(newAngle.y, -this->maxPhi), this->maxPhi);
        }

        if (this->angle != newAngle)
        {
            this->angle = newAngle;
            this->updatePosition();
        }

    }

    Camera::update(force);
}

void OrbitCamera::setPosition(const vec3 &pos)
{
    auto p = pos - this->centerPosition;
    const float d = length(p);
    p /= d;

    const float phi = asin(p.y);
    const float theta = p.z != 0 ? atan2(p.x, p.z) : (p.x >= 0 ? 0 : pi<float>());

    this->angle = vec2(theta, phi);
    this->distance = d;
    this->updatePosition();
}


void OrbitCamera::updatePosition()
{
    float cosPhi = cos(this->angle.y);
    auto pos = vec3(cosPhi * sin(this->angle.x), sin(this->angle.y), cosPhi * cos(this->angle.x));
    pos *= this->distance;
    Object3D::setPosition(pos + this->centerPosition);
    this->lookAt(this->centerPosition);
}
