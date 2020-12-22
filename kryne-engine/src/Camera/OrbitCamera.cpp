/**
 * @file
 * @author Max Godefroy
 * @date 16/12/2020.
 */

#include "kryne-engine/Camera/OrbitCamera.h"

OrbitCamera::OrbitCamera(unique_ptr<ProjectionData> projectionData, weak_ptr<PlayerInput> playerInput) :
    Camera(move(projectionData)),
    playerInput(std::move(playerInput))
{
    this->position = glm::vec3(0, 0, 5);
}


void OrbitCamera::updateTransform(bool force)
{
    const auto input = this->playerInput.lock();

    bool needsAngleUpdate = false;
    bool needsCenterUpdate = false;
    bool needsDistanceUpdate = false;

    if (input != nullptr)
    {
        const auto dir = this->position - this->centerPosition;
        this->distance = length(dir);

        const auto movement = input->getCursorMovement();
        const auto scroll = input->getScrollInput();
        auto newAngle = this->angle;
        auto newCenter = this->centerPosition;
        auto newDistance = this->distance;

        if (movement.x != 0 && movement.y != 0)
        {
            if (input->isKeyDown(GLFW_MOUSE_BUTTON_1))
            {
                needsAngleUpdate = true;
                newAngle.x += -movement.x * this->radiansPerMousePixel;
                newAngle.y += movement.y * this->radiansPerMousePixel;
                newAngle.y = glm::min(glm::max(newAngle.y, -this->maxPhi), this->maxPhi);
            }
            else if (input->isKeyDown(GLFW_MOUSE_BUTTON_2))
            {
                needsCenterUpdate = true;

                const auto transformMatrix = Math::getLookAtMatrix(newCenter, this->position);

                newCenter += transformMatrix * vec3(-movement.x * this->radiansPerMousePixel, movement.y * this->radiansPerMousePixel, 0);
            }
        }

        if (scroll.y != 0)
        {
            needsDistanceUpdate = true;
            newDistance += -scroll.y * this->scrollZoomSpeed;
            newDistance = glm::max(0.0001f, newDistance);
        }

        if (needsAngleUpdate || needsCenterUpdate || needsDistanceUpdate)
        {
            this->angle = newAngle;
            this->centerPosition = newCenter;
            this->distance = newDistance;
            this->updatePosition();
        }

    }

    Camera::updateTransform(force || needsAngleUpdate || needsCenterUpdate || needsDistanceUpdate);
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
