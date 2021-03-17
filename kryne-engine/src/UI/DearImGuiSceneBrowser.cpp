/**
 * @file
 * @author Max Godefroy
 * @date 17/03/2021.
 */

#include <kryne-engine/Utils/DreamImGui.hpp>
#include "kryne-engine/UI/DearImGuiSceneBrowser.hpp"


using Utils::ImGuiLabel;


void DearImGuiSceneBrowser::renderComponent(Process *process)
{
    auto &io = ImGui::GetIO();

    {
        auto scene = process->getCurrentScene();

        float windowWidth = 400.f;
        float windowHeight = io.DisplaySize.y;

        ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Appearing);

        ImGui::Begin("Scene browser");

        ImGui::GetStyle().WindowRounding = 5.0f;

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Appearing);
        if (ImGui::TreeNode(ImGuiLabel("Scene", scene)))
        {
            for (const auto e : scene->getTopLevelEntities())
                this->displaySceneEntity(e);
            ImGui::TreePop();
        }

        ImGui::End();
    }

    if (this->selectedEntity)
    {
        float windowWidth = 320.f;
        float windowHeight = io.DisplaySize.y;
        float x = io.DisplaySize.x - windowWidth;

        ImGui::SetNextWindowPos(ImVec2(x, 0.f), ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Appearing);

        ImGui::Begin("Details");

        displayEntityInfo(this->selectedEntity);
        displayTransform(this->selectedEntity->getTransform());
        displayComponents(this->selectedEntity);

        ImGui::End();
    }
}


void DearImGuiSceneBrowser::displaySceneEntity(Entity *entity) 
{
    auto children = entity->getTransform()->getChildren();

    const uint32_t baseFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

    if (!entity->isEnabled())
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));

    if (children.empty())
    {
        uint32_t flags = baseFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        if (entity == this->selectedEntity)
            flags |= ImGuiTreeNodeFlags_Selected;

        ImGui::TreeNodeEx(entity, flags, "%s", entity->getName().c_str());

        if (ImGui::IsItemClicked())
            this->selectedEntity = (this->selectedEntity == entity) ? nullptr : entity;
    }
    else
    {
        uint32_t flags = baseFlags | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (entity == this->selectedEntity)
            flags |= ImGuiTreeNodeFlags_Selected;

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Appearing);
        bool open = ImGui::TreeNodeEx(entity, flags, "%s", entity->getName().c_str());
        bool clicked = ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen();

        if (open)
        {
            for (const auto &child : children)
                this->displaySceneEntity(child->getEntity());
            ImGui::TreePop();
        }

        if (clicked)
            this->selectedEntity = (this->selectedEntity == entity) ? nullptr : entity;
    }

    if (!entity->isEnabled())
        ImGui::PopStyleColor();
}


int DearImGuiSceneBrowser::updateName(ImGuiInputTextCallbackData *data) 
{
    auto entity = (Entity *)(data->UserData);
    string name = data->Buf;
    entity->setName(name);
    return 0;
}


void DearImGuiSceneBrowser::displayEntityInfo(Entity *entity) 
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
    if (ImGui::CollapsingHeader(ImGuiLabel("Entity", entity)))
    {
        {
            bool enabled = entity->isEnabled();
            ImGui::Checkbox(ImGuiLabel("Enabled", entity), &enabled);
            entity->setEnabled(enabled);
        }

        {
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Name:");

            ImGui::SameLine();

            char name[2048];
            sprintf(name,"%s", entity->getName().c_str());

            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputTextWithHint(
                    ImGuiLabel("##Name", entity), "Enter name",
                    name, IM_ARRAYSIZE(name),
                    ImGuiInputTextFlags_CallbackEdit, DearImGuiSceneBrowser::updateName, entity);
        }

        ImGui::Separator();
    }
}

void DearImGuiSceneBrowser::displayTransform(Transform *transform) 
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);

    if (ImGui::CollapsingHeader(ImGuiLabel("Transform", transform)))
    {
        {
            vec3 position = transform->getPosition();
            ImGui::DragFloat3(ImGuiLabel("Position", transform), value_ptr(position), .01f);
            transform->setPosition(position);
        }

        {
            vec3 scale = transform->getScale();
            ImGui::DragFloat3(ImGuiLabel("Scale", transform), value_ptr(scale), .01f);
            transform->setScale(scale);
        }

        {
            vec3 rotation = degrees(transform->getEuler());
            ImGui::DragFloat3(ImGuiLabel("Rotation", transform), value_ptr(rotation), .5f, 0.f, 0.f, "%.1f°");
            transform->setEuler(radians(rotation));
        }

        ImGui::Separator();
    }
}


void DearImGuiSceneBrowser::displayComponents(Entity *entity) 
{
    for (auto component : entity->getAllComponents())
    {
        ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);

        if (!component->isEnabled())
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));

        bool open = ImGui::CollapsingHeader(component->getComponentName().c_str());

        if (!component->isEnabled())
            ImGui::PopStyleColor();

        if (open)
        {
            {
                bool enabled = component->isEnabled();
                ImGui::Checkbox(ImGuiLabel("Enabled", component), &enabled);
                component->setEnabled(enabled);
            }

            ImGui::Separator();
        }
    }
}
