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
            ImGui::DragFloat3("##direction", value_ptr(dir), 0.01, -1, 1);

            if (ImGui::IsItemEdited())
                this->setDirection(dir);
        }

        {
            bool cs = this->castShadow;

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Cast shadow: ");
            ImGui::TableNextColumn();
            ImGui::Checkbox("##castShadow", &cs);

            if (cs != this->castShadow)
                this->setCastShadow(cs);
        }

        if (this->castShadow)
        {
            {
                float maxDist = this->maxShadowDistance;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Max shadow distance: ");
                ImGui::TableNextColumn();
                ImGui::DragFloat("##maxDist", &maxDist, 0.1, 0, 1000, "%.1f");

                if (ImGui::IsItemEdited())
                    this->setMaxShadowDistance(maxDist);
            }

            {
                float minDepth = this->minShadowDepth;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Min shadow depth: ");
                ImGui::TableNextColumn();
                ImGui::DragFloat("##minDepth", &minDepth, 0.1, 0, 1000, "%.1f");

                if (ImGui::IsItemEdited())
                    this->setMinShadowDepth(minDepth);
            }

            {
                int csm = this->cascadedShadowMaps;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Shadow cascades: ");
                ImGui::TableNextColumn();
                ImGui::DragInt("##csm", &csm, 0.01, 0, 4);

                if (ImGui::IsItemEdited())
                    this->setCascadedShadowMaps(csm);
            }
        }

        ImGui::EndTable();
    }
}
