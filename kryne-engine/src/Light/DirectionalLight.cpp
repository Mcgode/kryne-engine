/**
 * @file
 * @author Max Godefroy
 * @date 06/04/2021.
 */

#include "kryne-engine/Light/DirectionalLight.hpp"

DirectionalLight::DirectionalLight(Process *process,
                                   const vec3 &color,
                                   float intensity,
                                   const vec3 &direction)
        : Light(process, LightType::DirectionalLight),
          color(color),
          intensity(intensity),
          direction(normalize(direction)),
          worldDirection(normalize(direction))
{
    this->name = "DirectionalLight";
}


void DirectionalLight::transformDidUpdate()
{
    Entity::transformDidUpdate();
    worldDirection = this->getTransform()->getNormalMatrix() * this->direction;
}


void DirectionalLight::renderEntityDetails()
{
    Entity::renderEntityDetails();

    if (ImGui::BeginTable("DirectionalLightParams", 2))
    {
        ImGui::TableSetupColumn("##labels", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("##values", ImGuiTableColumnFlags_WidthStretch);

        {
            auto col = this->color;

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Color:");
            ImGui::TableNextColumn();
            ImGui::ColorEdit3("##color", value_ptr(col), ImGuiColorEditFlags_NoInputs);

            if (ImGui::IsItemEdited())
                this->setColor(col);
        }

        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Intensity: ");
            ImGui::TableNextColumn();
            ImGui::DragFloat("##intensity", &intensity, 0.03f, 0.f, FLT_MAX, "%.2f");
        }

        {
            vec3 dir = this->direction;

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Direction: ");
            ImGui::TableNextColumn();
            ImGui::DragFloat3("##intensity", value_ptr(dir), 0.01, -1, 1);

            if (ImGui::IsItemEdited())
                this->setDirection(dir);
        }

        ImGui::EndTable();
    }
}
