/**
 * @file
 * @author Max Godefroy
 * @date 16/12/2020.
 */

#include "kryne-engine/Camera/OrbitCamera.h"

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

    bool needAngleUpdate = false;
    bool needCenterUpdate = false;

    if (input != nullptr)
    {
        const auto dir = this->position - this->centerPosition;
        this->distance = length(dir);

        const auto movement = input->getCursorMovement();
        auto newAngle = this->angle;
        auto newCenter = this->centerPosition;

        if (movement.x != 0 && movement.y != 0)
        {
            if (input->isKeyDown(GLFW_MOUSE_BUTTON_1))
            {
                needAngleUpdate = true;
                newAngle.x += -movement.x * this->radiansPerMousePixel;
                newAngle.y += movement.y * this->radiansPerMousePixel;
                newAngle.y = glm::min(glm::max(newAngle.y, -this->maxPhi), this->maxPhi);
            }
            else if (input->isKeyDown(GLFW_MOUSE_BUTTON_2))
            {
                needCenterUpdate = true;

                const vec3 up(0, 1, 0);
                auto z = normalize(this->position - newCenter);
                auto x = cross(up, z);

                if (glm::length(x) == 0.f) {
                    if (glm::abs(z.z) == 1.0f)
                        z.x += 0.0001f;
                    else
                        z.z += 0.0001f;

                    z = glm::normalize(z);
                    x = glm::normalize(glm::cross(up, z));
                } else
                    x = glm::normalize(x);

                auto y = glm::cross(z, x);

                const float dx = -movement.x * this->radiansPerMousePixel;
                const float dy = movement.y * this->radiansPerMousePixel;
                newCenter += dx * x + dy * y;
            }
        }

        if (needAngleUpdate || needCenterUpdate)
        {
            this->angle = newAngle;
            this->centerPosition = newCenter;
            this->updatePosition();
        }

    }

    Camera::update(force || needAngleUpdate || needCenterUpdate);
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
