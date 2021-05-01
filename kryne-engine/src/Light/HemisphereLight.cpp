/**
 * @file
 * @author Max Godefroy
 * @date 05/04/2021.
 */

#include "kryne-engine/Light/HemisphereLight.hpp"


HemisphereLight::HemisphereLight(Process *process,
                                 const vec3 &skyColor,
                                 const vec3 &groundColor,
                                 float intensity,
                                 const vec3 &direction) :
    Light(process, LightType::HemisphereLight),
    skyColor(skyColor),
    groundColor(groundColor),
    intensity(intensity),
    direction(normalize(direction)),
    worldDirection(normalize(direction))
{
    this->name = "HemisphereLight";
}


void HemisphereLight::transformDidUpdate()
{
    Entity::transformDidUpdate();
    this->worldDirection = this->transform->getNormalMatrix() * this->direction;
}

void HemisphereLight::renderEntityDetails()
{
    Entity::renderEntityDetails();

    if (ImGui::BeginTable("DirectionalLightParams", 2))
    {
        ImGui::TableSetupColumn("##labels", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("##values", ImGuiTableColumnFlags_WidthStretch);

        {
            auto col = this->skyColor;

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Sky color:");
            ImGui::TableNextColumn();
            ImGui::ColorEdit3("##sky_color", value_ptr(col), ImGuiColorEditFlags_NoInputs);

            if (ImGui::IsItemEdited())
                this->setSkyColor(col);
        }

        {
            auto col = this->groundColor;

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Ground color:");
            ImGui::TableNextColumn();
            ImGui::ColorEdit3("##ground_color", value_ptr(col), ImGuiColorEditFlags_NoInputs);

            if (ImGui::IsItemEdited())
                this->setGroundColor(col);
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
            ImGui::DragFloat3("##direction", value_ptr(dir), 0.01, -1, 1);

            if (ImGui::IsItemEdited())
                this->setDirection(dir);
        }

        ImGui::EndTable();
    }
}
