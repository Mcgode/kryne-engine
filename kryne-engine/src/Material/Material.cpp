//
// Created by Max Godefroy on 21/11/2020.
//

#include <imgui.h>

#include "kryne-engine/Material/Material.h"


void Material::prepareShader(const BufferGeometry *geometry)
{
    if (this->shader->getNeedsUpdate())
        this->shader->notifyUniformLocationsNeedUpdate();
    this->compiler->checkShaderCompile(this->shader.get(), geometry);
    this->shader->use();
    this->beforeUpload(geometry);
    this->shader->updateUniforms();
}


void Material::displayDearImGui(Process *process)
{
    auto name = this->materialName();

    if (!name.empty())
        name = "Material (" + name + ")";
    else
        name = "Material";

    if (ImGui::CollapsingHeader(name.c_str()))
        this->dearImGuiData(process);
}
