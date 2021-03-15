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


void displayEntity(Entity *entity)
{
    auto children = entity->getTransform()->getChildren();

    stringstream fmt;
    fmt << "0x" << hex << entity;
    auto name = fmt.str();

    if (children.empty())
        ImGui::Selectable(name.c_str());
    else
    {
        if (ImGui::TreeNode(name.c_str()))
        {
            for (const auto &child : children)
                displayEntity(child->getEntity());
            ImGui::TreePop();
        }
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
        const auto mesh = entity->addComponent<RenderMesh>(geometry, material);
        entity->getTransform()->setScene(scene);
    });

    const auto camera = process->makeEntity<Camera>(make_unique<PerspectiveProjectionData>(16.f / 9.f));
    camera->addComponent<OrbitControlsComponent>();
    camera->getTransform()->setScene(scene);
    process->getGraphicContext()->getRenderer()->setCamera(camera);

    const auto copyMaterial = make_shared<TextureCopyMaterial>();
    auto pass = make_unique<ShaderPass>("CopyPass", copyMaterial);
    process->getGraphicContext()->getRenderer()->addPass(move(pass));

//    auto context = dynamic_cast<OpenGLContext *>(process->getGraphicContext());
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGui_ImplGlfw_InitForOpenGL(context->getWindow(), true);
//    ImGui_ImplOpenGL3_Init("#version 330 core");
//    ImGui::StyleColorsDark();
    process->getUIRenderers().emplace_back(new DearIMGUIPrototype(context->getWindow(), [](Process *process)
    {
        ImGui::Begin("Scene browser");

//        ImGui::SetNextTreeNodeOpen(true);
        if (ImGui::TreeNode("Scene"))
        {
            for (const auto e : process->getCurrentScene()->getTopLevelEntities())
                displayEntity(e);
            ImGui::TreePop();
        }

        ImGui::End();

        ImGui::ShowDemoWindow();
    }));

    using namespace std::chrono;
    uint64_t start = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    double t;
    while (!process->getGraphicContext()->shouldStop()) {

//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();

        uint64_t uit = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
        t = (uit - start);
        t /= 1000.0;

//        material->getShader()->debugPrintActiveUniforms();

        auto lookPos = glm::vec3(2.*glm::cos(t), 1, 2.*glm::sin(t));
//        mesh->lookAt(lookPos);

        process->runLoop();

//        ImGui::Begin("Demo window");
//        ImGui::Button("Hello!");
//        ImGui::End();
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();

    return 0;
}