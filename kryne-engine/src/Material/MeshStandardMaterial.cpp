/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#include <imgui.h>

#include "kryne-engine/Material/MeshStandardMaterial.h"

MeshStandardMaterial::MeshStandardMaterial(const StandardInitParameters &options):
    MeshMaterialCommon(make_unique<Shader>("Engine/Standard"))
{
    this->initializeCommon(options);
    this->setRoughness(options.roughness);
    this->setMetalness(options.metalness);
    this->setNormalMap(options.normalMap);
    this->setRoughnessMap(options.roughnessMap);
    this->setMetalnessMap(options.metalnessMap);
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"


void MeshStandardMaterial::dearImGuiData()
{
    if (ImGui::BeginTable("MaterialTable", 2, ImGuiTableFlags_BordersInnerH))
    {
        ImGui::TableSetupColumn("##labels", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("##values", ImGuiTableColumnFlags_WidthStretch);


        {
            auto color = this->color;

            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ImGui::Text("Color"); ImGui::TableNextColumn();
            ImGui::ColorEdit3("##color", value_ptr(color), ImGuiColorEditFlags_NoInputs);

            if (ImGui::IsItemEdited())
                this->setColor(color);
        }

        {
            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ImGui::Text("Albedo map"); ImGui::TableNextColumn();
            if (map)
                ImGui::Image((void *)map->getId(), ImVec2(64, 64));
            else
                ImGui::Text("None");
        }

        {
            float rough = this->roughness;

            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ImGui::Text("Roughness"); ImGui::TableNextColumn();
            ImGui::DragFloat("##roughness", &rough, .01, 0, 1);
            if (ImGui::IsItemEdited())
                this->setRoughness(rough);
        }

        {
            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ImGui::Text("Roughness map"); ImGui::TableNextColumn();
            if (this->roughnessMap)
                ImGui::Image((void *)this->roughnessMap->getId(), ImVec2(64, 64));
            else
                ImGui::Text("None");
        }

        {
            float metal = this->metalness;

            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ImGui::Text("Metalness"); ImGui::TableNextColumn();
            ImGui::DragFloat("##metalness", &metal, .01, 0, 1);
            if (ImGui::IsItemEdited())
                this->setMetalness(metal);
        }

        {
            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ImGui::Text("Metalness map"); ImGui::TableNextColumn();
            if (this->metalnessMap)
                ImGui::Image((void *)this->metalnessMap->getId(), ImVec2(64, 64));
            else
                ImGui::Text("None");
        }

        {
            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ImGui::Text("Normal map"); ImGui::TableNextColumn();
            if (this->normalMap)
                ImGui::Image((void *)this->normalMap->getId(), ImVec2(64, 64));
            else
                ImGui::Text("None");
        }

        ImGui::EndTable();
    }
}


#pragma clang diagnostic pop
