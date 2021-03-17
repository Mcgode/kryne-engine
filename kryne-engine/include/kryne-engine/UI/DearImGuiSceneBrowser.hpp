/**
 * @file
 * @author Max Godefroy
 * @date 17/03/2021.
 */

#pragma once


#include "DearIMGUI.h"


class DearImGuiSceneBrowser : public DearIMGUI {

public:

    explicit DearImGuiSceneBrowser(GLFWwindow *window) : DearIMGUI(window) {}

protected:

    void defineGUI(Process *process) override;

    virtual void displaySceneEntity(Entity *entity);

    static int updateName(ImGuiInputTextCallbackData *data);

    virtual void displayEntityInfo(Entity *entity);

    virtual void displayTransform(Transform *transform);

    virtual void displayComponents(Entity *entity);

protected:

    Entity *selectedEntity = nullptr;

};


