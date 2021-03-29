/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#include <imgui.h>

#include "kryne-engine/Material/MeshLambertMaterial.h"


MeshLambertMaterial::MeshLambertMaterial(const InitParameters &options):
    MeshMaterialCommon(make_unique<Shader>("Engine/Lambert"))
{
    this->initializeCommon(options);
}

#define ImageID(id) reinterpret_cast<void *>((unsigned long long)id)

void MeshLambertMaterial::dearImGuiData()
{
    if (ImGui::BeginTable("MaterialTable", 2, ImGuiTableFlags_BordersInnerH))
    {
        ImGui::TableSetupColumn("##labels", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("##values", ImGuiTableColumnFlags_WidthStretch);

        {
            auto color = this->color;

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Color");
            ImGui::TableNextColumn();
            ImGui::ColorEdit3("##color", value_ptr(color), ImGuiColorEditFlags_NoInputs);

            if (ImGui::IsItemEdited())
                this->setColor(color);
        }

        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Albedo map");
            ImGui::TableNextColumn();
            if (map)
                ImGui::Image(ImageID(map->getId()), ImVec2(64, 64));
            else
                ImGui::Text("None");
        }

        ImGui::EndTable();
    }
}
