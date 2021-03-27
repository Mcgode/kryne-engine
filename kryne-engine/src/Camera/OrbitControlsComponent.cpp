/**
 * @file
 * @author Max Godefroy
 * @date 16/12/2020.
 */

#include <kryne-engine/Utils/DreamImGui.hpp>

#include "kryne-engine/Camera/OrbitControlsComponent.h"

OrbitControlsComponent::OrbitControlsComponent(Entity *entity) :
    LogicComponent(entity)
{
    this->setPosition(vec3(0, 0, 5));
    this->componentName = "OrbitControlsComponent";
}


void OrbitControlsComponent::onUpdate()
{
    const auto input = this->associatedEntity->getProcess()->getPlayerInput();

    bool needsAngleUpdate = false;
    bool needsCenterUpdate = false;
    bool needsDistanceUpdate = false;

    const auto currentPosition = this->getTransform()->getPosition();
    const auto dir = currentPosition - this->centerPosition;
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

            const auto transformMatrix = Math::getLookAtMatrix(newCenter, currentPosition);

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

void OrbitControlsComponent::setPosition(const vec3 &pos)
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


void OrbitControlsComponent::updatePosition()
{
    float cosPhi = cos(this->angle.y);
    auto pos = vec3(cosPhi * sin(this->angle.x), sin(this->angle.y), cosPhi * cos(this->angle.x));
    pos *= this->distance;
    this->getTransform()->setPosition(pos + this->centerPosition);
    this->getTransform()->lookAt(this->centerPosition, vec3(0, 1, 0), true);
}


void OrbitControlsComponent::renderComponentDetails()
{
    using Utils::ImGuiLabel;

    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Mouse speed:");
        ImGui::SameLine();

        ImGui::SetNextItemWidth(130.f);
        ImGui::DragFloat(ImGuiLabel("speed", this, true), &this->radiansPerMousePixel,
                         0.0001f, 0.f, 0.1f, "%.4f units/px");
    }

    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Maximum vertical angle:");
        ImGui::SameLine();

        ImGui::SetNextItemWidth(50.f);

        float max_angle = glm::degrees(this->maxPhi);
        ImGui::DragFloat(ImGuiLabel("max_angle", this, true), &max_angle,
                         0.1f, 0.f, 89.9f, "%.1f°");
        if (ImGui::IsItemEdited())
        {
            this->maxPhi = glm::radians(max_angle);
            this->angle.y = glm::min(glm::max(angle.y, -this->maxPhi), this->maxPhi);
            this->updatePosition();
        }
    }

    {
        ImGui::SetNextItemWidth(200.f);
        vec3 position = this->getTransform()->getPosition();
        ImGui::DragFloat3(ImGuiLabel("Local position", this), value_ptr(position), .01f);
        if (ImGui::IsItemEdited())
            this->setPosition(position);
    }

    {
        ImGui::SetNextItemWidth(200.f);
        vec3 position = this->centerPosition;
        ImGui::DragFloat3(ImGuiLabel("Center position", this), value_ptr(position), .01f);
        if (ImGui::IsItemEdited())
            this->setCenterPosition(position);
    }

    {
        ImGui::SetNextItemWidth(100.f);
        vec3 position = this->centerPosition;
        ImGui::DragFloat(ImGuiLabel("Distance to center", this), &this->distance, .01f, 0.01f, 1000.f);
        if (ImGui::IsItemEdited())
            this->updatePosition();
    }
}
