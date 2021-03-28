/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#include <kryne-engine/Core/Process.h>
#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>
#include <kryne-engine/Systems/TransformUpdateSystem.h>
#include <kryne-engine/Systems/GameLogicComponentsRunner.h>
#include <kryne-engine/Material/MeshStandardMaterial.h>
#include <kryne-engine/Geometry/BoxBufferGeometry.h>
#include <kryne-engine/Camera/PerspectiveProjectionData.hpp>
#include <kryne-engine/Camera/OrbitControlsComponent.h>
#include <kryne-engine/UI/DearImGui.h>
#include <kryne-engine/UI/DearImGuiPrototype.hpp>
#include <kryne-engine/UI/DearImGuiPerformanceMetrics.hpp>
#include <kryne-engine/UI/DearImGuiSceneBrowser.hpp>
#include "WorkComponent.hpp"


void setBestCase(const vector<Entity *> &entities, Scene *scene)
{
    for (auto &entity : entities)
    {
        entity->getTransform()->removeFromParent();
    }

    for (auto &entity : entities)
    {
        entity->getTransform()->setScene(scene);
    }
}


void setWorstCase(const vector<Entity *> &entities, Scene *scene)
{
    auto entity = entities[0];
    entity->getTransform()->removeFromParent();
    entity->getTransform()->setScene(scene);

    for (auto i = 1; i < entities.size(); i++)
    {
        entity = entities[i];
        entities[i - 1]->getTransform()->add(entity->getTransform());
    }
}


int main()
{
    const auto context = new OpenGLContext();
    const auto process = make_unique<Process>(context);
    const auto scene = process->makeScene();
    process->setCurrentScene(scene);
    process->makeSystem<TransformUpdateSystem>();
    process->makeSystem<GameLogicComponentsRunner>();

    vector<Entity *> entities {};
    for (auto i = 0; i < 100; i++)
    {
        const auto entity = process->makeEntity<Entity>();
        entity->getTransform()->setScene(scene);
        entity->addComponent<WorkComponent>();
        entities.push_back(entity);
    }

    const auto camera = process->makeEntity<Camera>(make_unique<PerspectiveProjectionData>(16.f / 9.f));
    camera->addComponent<OrbitControlsComponent>();
    camera->getTransform()->setScene(scene);
    camera->setName("OrbitCamera");
    process->getGraphicContext()->getRenderer()->setCamera(camera);

    auto dearImGui = dynamic_cast<DearImGui *>(process->getUIRenderers().emplace_back(new DearImGui(context->getWindow())));

    dearImGui->getComponents().emplace_back(new DearImGuiSceneBrowser());
    dearImGui->getComponents().emplace_back(new DearImGuiPerformanceMetrics());

    int mode = 0;
    dearImGui->getComponents().emplace_back(new DearImGuiPrototype([&mode, &entities, &scene](Process *process)
    {
        ImGui::Begin("Controls");

        int i = mode;
        ImGui::RadioButton("Best case", &i, 0); ImGui::SameLine();
        ImGui::RadioButton("Worst case", &i, 1);
        if (i != mode)
        {
            if (i)
                setWorstCase(entities, scene);
            else
                setBestCase(entities, scene);
        }
        mode = i;

        ImGui::End();

        ImGui::ShowDemoWindow();
    }));

    while (!process->getGraphicContext()->shouldStop())
    {
        process->runLoop();
    }

    return 0;
}