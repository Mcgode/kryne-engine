//
// Created by Max Godefroy on 22/11/2020.
//

#define KRYNE_ENGINE_MAIN_THREAD_ASSERT_DISABLE

#include <algorithm>
#include <chrono>
#include <KEModules/Rendering.h>
#include <KEModules/Camera.h>
#include <KEModules/Geometry.h>
#include <KEModules/Material.h>
#include <KEModules/Textures.h>
#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>
#include <kryne-engine/Systems/TransformUpdateSystem.h>
#include <kryne-engine/Systems/GameLogicComponentsRunner.h>
#include <kryne-engine/Systems/LightingRegistrySystem.hpp>
#include <kryne-engine/Material/AdditionalMaterials/FXAAMaterial.hpp>
#include <kryne-engine/Rendering/Additional/ShaderPass.h>
#include <kryne-engine/UI/DearImGui.h>
#include <kryne-engine/UI/DearImGuiPrototype.hpp>
#include <kryne-engine/UI/DearImGuiSceneBrowser.hpp>
#include <kryne-engine/UI/DearImGuiPerformanceMetrics.hpp>


int main()
{
    cout << "Is main thread: " << Dispatcher::instance().main()->isCurrentThread() << endl;

    const auto context = new OpenGLContext();
    const auto process = make_unique<Process>(context);
    const auto scene = process->makeScene();
    process->setCurrentScene(scene);
    process->makeSystem<TransformUpdateSystem>();
    process->makeSystem<GameLogicComponentsRunner>();
    process->makeSystem<LightingRegistrySystem>();

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

    const auto light = process->makeEntity<HemisphereLight>(vec3(1, 0, 0), vec3(0.1, 0, 0), 0.5f, vec3(-1, 0, 0));
    light->getTransform()->setScene(scene);

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

        auto geometry = dynamic_pointer_cast<BufferGeometry>(make_shared<BoxBufferGeometry>());
        const auto entity = process->makeEntity<Entity>();
        entity->getTransform()->setScene(scene);

        const vec3 offsets[4] = {
            vec3(-2, 0, -2),
            vec3(-2, 0, 2),
            vec3(2, 0, -2),
            vec3(2, 0, 2),
        };

        for (uint32_t i = 0; i < 4; i++)
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

    const auto fxaaMaterial = make_shared<FXAAMaterial>();
    auto pass = make_unique<ShaderPass>("FXAAPass", fxaaMaterial, PostProcessPass::Priority::Antialiasing);
    process->getGraphicContext()->getRenderer()->addPass(move(pass));

    auto dearImGui = dynamic_cast<DearImGui *>(process->getUIRenderers().emplace_back(new DearImGui(context->getWindow())));

    dearImGui->getComponents().emplace_back(new DearImGuiSceneBrowser());
    dearImGui->getComponents().emplace_back(new DearImGuiPerformanceMetrics());

    dearImGui->getComponents().emplace_back(new DearImGuiPrototype([](Process *process)
    {
        ImGui::ShowDemoWindow();
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