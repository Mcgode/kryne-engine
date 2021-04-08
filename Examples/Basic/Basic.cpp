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

    const auto map = Texture2D::loadFromFile("Resources/Textures/cobblestone/cobblestone_floor_diff.jpg");
    const auto normalMap = Texture2D::loadFromFile("Resources/Textures/cobblestone/cobblestone_floor_norm.jpg");
    const auto roughnessMap = Texture2D::loadFromFile("Resources/Textures/cobblestone/cobblestone_floor_rough.jpg");

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

    vector<string> skyTextureFiles = {
        "Resources/Textures/skybox/right.tga",
        "Resources/Textures/skybox/left.tga",
        "Resources/Textures/skybox/top.tga",
        "Resources/Textures/skybox/bottom.tga",
        "Resources/Textures/skybox/front.tga",
        "Resources/Textures/skybox/back.tga",
    };
    const auto skyEnvMap = EnvironmentMap::loadFiles(skyTextureFiles);
    scene->setSkyboxEnvMap(skyEnvMap);

    MeshStandardMaterial::StandardInitParameters params;

    const auto hemisphereLight = process->makeEntity<HemisphereLight>(vec3(1),
                                                                      vec3(0.5),
                                                                      0.75f,
                                                                      vec3(0, 1, 0));
//    hemisphereLight->getTransform()->setScene(scene);

    const auto directionalLight = process->makeEntity<DirectionalLight>(vec3(1, 1, 1),
                                                                        1.5f,
                                                                        vec3(-1, -1, -1));
    directionalLight->getTransform()->setScene(scene);

    Dispatcher::instance().enqueueParallelDelayed([&process, &params, &scene, map, normalMap, roughnessMap, &skyEnvMap]()
    {
        params.map = map;
        params.normalMap = normalMap;
        params.roughnessMap = roughnessMap;
        params.metalness = 0.0;
        auto material = make_shared<MeshStandardMaterial>(params);
        material->setSide(FrontSide);
        material->setEnvMap(skyEnvMap);

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

    while (!process->getGraphicContext()->shouldStop())
    {
        process->runLoop();
    }

    return 0;
}