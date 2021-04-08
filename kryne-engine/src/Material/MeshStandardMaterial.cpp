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

    this->setEnvMapIntensity(1);
}

// Cast GLuint to void * without compiler warning
#define ImageID(id) reinterpret_cast<void *>((unsigned long long)id)


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
                ImGui::Image(ImageID(map->getId()), ImVec2(64, 64));
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
                ImGui::Image(ImageID(this->roughnessMap->getId()), ImVec2(64, 64));
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
                ImGui::Image(ImageID(this->metalnessMap->getId()), ImVec2(64, 64));
            else
                ImGui::Text("None");
        }

        {
            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ImGui::Text("Normal map"); ImGui::TableNextColumn();
            if (this->normalMap)
                ImGui::Image(ImageID(this->normalMap->getId()), ImVec2(64, 64));
            else
                ImGui::Text("None");
        }

        ImGui::EndTable();
    }
}


void MeshStandardMaterial::beforeUpload(const BufferGeometry *geometry)
{
    if (this->envMap != nullptr && this->envMap->getIblEnvMap() != this->iblEnvMap)
        this->setMaterialOptionalProperty(this->iblEnvMap, this->envMap->getIblEnvMap(),
                                          "envMap", "USE_ENVMAP");
}
