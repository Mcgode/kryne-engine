/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#include <kryne-engine/Utils/DreamImGui.hpp>

#include "kryne-engine/Rendering/RenderMesh.h"


RenderMesh::RenderMesh(Entity *entity, shared_ptr<BufferGeometry> geometry, shared_ptr<Material> material):
    Component(entity),
    geometry(move(geometry)),
    material(move(material))
{
    this->componentName = "RenderMesh";
}


void RenderMesh::transformDidUpdate()
{
    auto transform = this->getEntity()->getTransform();
    if (this->geometry != nullptr)
        this->boundingSphere = this->geometry->getBoundingSphere().fromMatrix(transform->getWorldMatrix());
    else
        this->boundingSphere = Math::Sphere();
}


void RenderMesh::renderComponentDetails()
{
    using Utils::ImGuiLabel;

    ImGui::Separator();

    ImGui::Checkbox(ImGuiLabel("Frustum culled", this), &this->frustumCull);

    if (this->geometry)
    {
        if (ImGui::TreeNode(this, "Geometry info"))
        {
            for (auto &line : this->geometry->getInfo())
                ImGui::Text("%s", line.c_str());

            ImGui::TreePop();
        }
    }
    else
    {
        ImGui::TextDisabled("No geometry attached");
    }

    // TODO: Material UI

}
