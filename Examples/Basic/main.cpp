//
// Created by Max Godefroy on 22/11/2020.
//

#define KRYNE_ENGINE_MAIN_THREAD_ASSERT_DISABLE

#include <chrono>
#include <KEModules/Rendering.h>
#include <KEModules/Camera.h>
#include <KEModules/Geometry.h>
#include <KEModules/Material.h>
#include <KEModules/Textures.h>
#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>
#include <kryne-engine/Systems/TransformUpdateSystem.h>
#include <kryne-engine/Systems/GameLogicComponentsRunner.h>
#include <kryne-engine/Material/AdditionalMaterials/TextureCopyMaterial.h>
#include <kryne-engine/Rendering/Additional/ShaderPass.h>
#include <kryne-engine/UI/DearIMGUIPrototype.hpp>


Entity *selectedEntity = nullptr;


void displayEntityNode(Entity *entity)
{
    auto children = entity->getTransform()->getChildren();

    stringstream fmt;
    fmt << "0x" << hex << entity;
    auto name = fmt.str();
    const uint32_t baseFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

    if (!entity->isEnabled())
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));

    if (children.empty())
    {
        uint32_t flags = baseFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        if (entity == selectedEntity)
            flags |= ImGuiTreeNodeFlags_Selected;

        ImGui::TreeNodeEx(entity, flags, "%s", entity->getName().c_str());

        if (ImGui::IsItemClicked())
            selectedEntity = (selectedEntity == entity) ? nullptr : entity;
    }
    else
    {
        uint32_t flags = baseFlags | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (entity == selectedEntity)
            flags |= ImGuiTreeNodeFlags_Selected;

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Appearing);
        bool open = ImGui::TreeNodeEx(entity, flags, "%s", entity->getName().c_str());
        bool clicked = ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen();

        if (open)
        {
            for (const auto &child : children)
                displayEntityNode(child->getEntity());
            ImGui::TreePop();
        }

        if (clicked)
            selectedEntity = (selectedEntity == entity) ? nullptr : entity;
    }

    if (!entity->isEnabled())
        ImGui::PopStyleColor();
}


int updateName(ImGuiInputTextCallbackData *data)
{
    auto entity = (Entity *)(data->UserData);
    string name = data->Buf;
    entity->setName(name);
    return 0;
}


void displayEntityInfo(Entity *entity)
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
    if (ImGui::CollapsingHeader("Entity"))
    {
        {
            bool enabled = entity->isEnabled();
            ImGui::Checkbox("Enabled", &enabled);
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
                    "##", "Enter name",
                    name, IM_ARRAYSIZE(name),
                    ImGuiInputTextFlags_CallbackEdit, updateName, entity);
        }

        ImGui::Separator();
    }
}


int main()
{
    cout << "Is main thread: " << Dispatcher::instance().main()->isCurrentThread() << endl;

    const auto context = new OpenGLContext();
    const auto process = make_unique<Process>(context);
    const auto scene = process->makeScene();
    process->setCurrentScene(scene);
    process->makeSystem<TransformUpdateSystem>();
    process->makeSystem<GameLogicComponentsRunner>();

    const auto map = Texture2D::loadFromFileSync("Resources/Textures/cobblestone/cobblestone_floor_diff.jpg");
    const auto normalMap = Texture2D::loadFromFileSync("Resources/Textures/cobblestone/cobblestone_floor_norm.jpg");
    const auto roughnessMap = Texture2D::loadFromFileSync("Resources/Textures/cobblestone/cobblestone_floor_rough.jpg");

    // Uncomment to test multi-threaded texture loading
//    shared_ptr<Texture2D> maps[50];
//    for (int i = 0; i < 50; i++)
//    {
//        maps[i] = Texture2D::loadFromFileSync("Resources/Textures/cobblestone/cobblestone_floor_rough.jpg");
//    }
//    Dispatcher::instance().io()->enqueue([]()
//    {
//        cout << "IO operations should have concluded" << endl;
//    });

    MeshStandardMaterial::StandardInitParameters params;

    Dispatcher::instance().enqueueParallelDelayed([&process, &params, &scene, map, normalMap, roughnessMap]()
    {
        params.map = map;
        params.normalMap = normalMap;
        params.roughnessMap = roughnessMap;
        params.metalness = 0.0;
        auto material = make_shared<MeshStandardMaterial>(params);
        material->setSide(FrontSide);

        material->setDefine("MAX_DIRECTIONAL_LIGHTS", "1");
        material->setUniform("directionalLights[0].color", glm::vec3(1));
        material->setUniform("directionalLights[0].direction", glm::vec3(-1, -0.8, -0.5));

        const float intensity = 0.2;
        material->setDefine("MAX_HEMISPHERE_LIGHTS", "1");
        material->setUniform("hemisphereLights[0].skyColor", intensity * glm::vec3(1));
        material->setUniform("hemisphereLights[0].groundColor", intensity * glm::vec3(0.5));
        material->setUniform("hemisphereLights[0].direction", glm::vec3(0, 1, 0));

        auto geometry = dynamic_pointer_cast<BufferGeometry>(make_shared<BoxBufferGeometry>());
        const auto entity = process->makeEntity<Entity>();
        entity->getTransform()->setScene(scene);

        const vec3 offsets[4] = {
            vec3(-2, 0, -2),
            vec3(-2, 0, 2),
            vec3(2, 0, -2),
            vec3(2, 0, 2),
        };

        for (uint32_t i = 0; i < 2; i++)
        {
            auto cube = process->makeEntity<Entity>();
            cube->addComponent<RenderMesh>(geometry, material);
            auto transform = cube->getTransform();
            entity->getTransform()->add(transform);
            transform->setPosition(offsets[i]);
            cube->setName("Cube" + to_string(i));
        }
    });

    const auto camera = process->makeEntity<Camera>(make_unique<PerspectiveProjectionData>(16.f / 9.f));
    camera->addComponent<OrbitControlsComponent>();
    camera->getTransform()->setScene(scene);
    camera->setName("OrbitCamera");
    process->getGraphicContext()->getRenderer()->setCamera(camera);

    const auto copyMaterial = make_shared<TextureCopyMaterial>();
    auto pass = make_unique<ShaderPass>("CopyPass", copyMaterial);
    process->getGraphicContext()->getRenderer()->addPass(move(pass));

    process->getUIRenderers().emplace_back(new DearIMGUIPrototype(context->getWindow(), [](Process *process)
    {
        auto &io = ImGui::GetIO();

        {
            float windowWidth = 400.f;
            float windowHeight = io.DisplaySize.y;

            ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Appearing);
            ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Appearing);

            ImGui::Begin("Scene browser");

            ImGui::GetStyle().WindowRounding = 5.0f;

            ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Appearing);
            if (ImGui::TreeNode("Scene"))
            {
                for (const auto e : process->getCurrentScene()->getTopLevelEntities())
                    displayEntityNode(e);
                ImGui::TreePop();
            }

            ImGui::End();
        }

        if (selectedEntity)
        {
            float windowWidth = 320.f;
            float windowHeight = io.DisplaySize.y;
            float x = io.DisplaySize.x - windowWidth;

            ImGui::SetNextWindowPos(ImVec2(x, 0.f), ImGuiCond_Appearing);
            ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Appearing);

            ImGui::Begin("Details");

            displayEntityInfo(selectedEntity);

            ImGui::End();
        }

        {
            ImGui::ShowDemoWindow();
        }
    }));

    using namespace std::chrono;
    uint64_t start = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    double t;
    while (!process->getGraphicContext()->shouldStop())
    {
        uint64_t uit = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
        t = (uit - start);
        t /= 1000.0;

//        material->getShader()->debugPrintActiveUniforms();

        auto lookPos = glm::vec3(2.*glm::cos(t), 1, 2.*glm::sin(t));
//        mesh->lookAt(lookPos);

        process->runLoop();
    }

    return 0;
}