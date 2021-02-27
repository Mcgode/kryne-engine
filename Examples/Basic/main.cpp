//
// Created by Max Godefroy on 22/11/2020.
//

#include <chrono>
#include <KEModules/Rendering.h>
#include <KEModules/Camera.h>
#include <KEModules/Geometry.h>
#include <KEModules/Material.h>
#include <KEModules/Textures.h>
#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>
#include <kryne-engine/Systems/TransformUpdateSystem.h>
#include <kryne-engine/Systems/GameLogicComponentsRunner.h>

int main()
{
    cout << "Is main thread: " << Dispatcher::instance().main()->isCurrentThread() << endl;

    const auto process = make_unique<Process>(new OpenGLContext());
    const auto scene = process->makeScene();
    process->setCurrentScene(scene);
    process->makeSystem<TransformUpdateSystem>();
    process->makeSystem<GameLogicComponentsRunner>();

    const auto map = Texture2D::loadFromFileSync("Resources/Textures/cobblestone/cobblestone_floor_diff.jpg");
    const auto normalMap = Texture2D::loadFromFileSync("Resources/Textures/cobblestone/cobblestone_floor_norm.jpg");
    const auto roughnessMap = Texture2D::loadFromFileSync("Resources/Textures/cobblestone/cobblestone_floor_rough.jpg");

    MeshStandardMaterial::StandardInitParameters params;
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

    const auto camera = process->makeEntity<Camera>(make_unique<PerspectiveProjectionData>(16.f / 9.f));
    camera->addComponent<OrbitControlsComponent>();
    camera->getTransform()->setScene(scene);
    process->getGraphicContext()->getRenderer()->setCamera(camera);

    using namespace std::chrono;
    uint64_t start = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    double t;
    while (!process->getGraphicContext()->shouldStop()) {

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