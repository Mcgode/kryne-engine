/**
 * @file
 * @author Max Godefroy
 * @date 17/03/2021.
 */

#pragma once


#include "DearImGuiComponent.hpp"


class DearImGuiSceneBrowser : public DearImGuiComponent {

public:

    void renderComponent(Process *process) override;

protected:

    virtual void displaySceneEntity(Entity *entity);

    static int updateName(ImGuiInputTextCallbackData *data);

    virtual void displayEntityInfo(Entity *entity);

    virtual void displayTransform(Transform *transform);

    virtual void displayComponents(Entity *entity);

protected:

    Entity *selectedEntity = nullptr;

};


