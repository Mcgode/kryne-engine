//
// Created by Max Godefroy on 22/11/2020.
//

#include <chrono>
#include <KEModules/Rendering.h>
#include <KEModules/Camera.h>
#include <KEModules/Geometry.h>
#include <KEModules/Material.h>
#include <KEModules/Textures.h>

int main()
{
    auto renderer = make_unique<Renderer>(1280, 720);

    auto scene = make_unique<Scene>();

    const auto texture = Texture2D::loadFromFileSync("Resources/Textures/cobblestone/cobblestone_floor_diff.jpg");

    MeshStandardMaterial::StandardInitParameters params;
    params.color = glm::vec3(1, 0.4, 0);
    params.map = texture;
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

    const auto mesh = new Mesh(geometry, material);
    scene->add(unique_ptr<Object3D>(mesh));

    const auto camera = make_unique<OrbitCamera>(
        make_unique<PerspectiveProjectionData>(16.f / 9.f),
        renderer->getWeakPlayerInput()
    );

    const auto window = renderer->getWindow()->getGlfwWindow();
    using namespace std::chrono;
    uint64_t start = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    double t;
    while (!glfwWindowShouldClose(window)) {

        uint64_t uit = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
        t = (uit - start);
        t /= 1000.0;

//        material->getShader()->debugPrintActiveUniforms();

        auto lookPos = glm::vec3(2.*glm::cos(t), 1, 2.*glm::sin(t));
//        mesh->lookAt(lookPos);

        renderer->render(scene.get(), camera.get());
        glfwSwapBuffers(window);
        renderer->getPlayerInput()->willPollEvents();
        glfwPollEvents();
    }

    return 0;
}