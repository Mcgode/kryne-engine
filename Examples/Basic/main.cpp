//
// Created by Max Godefroy on 22/11/2020.
//

#include <chrono>
#include <kryne-engine/Camera/PerspectiveProjectionData.hpp>
#include <kryne-engine/Rendering/Renderer.h>
#include <kryne-engine/Geometry/BoxBufferGeometry.h>

int main()
{
    auto renderer = make_unique<Renderer>(1280, 720);

    auto scene = make_unique<Scene>();

    const string vertexShader = R"(
#include <vertex_pars_begin>
#include <light_pars_vertex>
#include <lambert_pars_vertex>

void main() {
    #include <vertex_begin>

    vec3 worldPosition = ( modelMatrix * vec4(transformed, 1.0) ).xyz;

    #include <vertex_position>
    #include <light_lambert_vertex>
})";

    const string fragmentShader =   R"(
in vec3 vNormal;
out vec4 FragColor;
uniform vec3 color;
#include <lambert_pars_fragment>

void main() {
    FragColor = vec4(color * (vLightDirectDiffuse + vLightIndirectDiffuse), 1);
})";

    auto material = make_shared<Material>(make_unique<Shader>(vertexShader, fragmentShader));
    material->setSide(FrontSide);

    material->setDefine("MAX_DIRECTIONAL_LIGHTS", "1");
    material->setUniform("directionalLights[0].color", glm::vec3(1));
    material->setUniform("directionalLights[0].direction", glm::vec3(-1, -0.8, -0.5));

    const float intensity = 0.2;
    material->setDefine("MAX_HEMISPHERE_LIGHTS", "1");
    material->setUniform("hemisphereLights[0].skyColor", intensity * glm::vec3(1));
    material->setUniform("hemisphereLights[0].groundColor", intensity * glm::vec3(0.5));
    material->setUniform("hemisphereLights[0].direction", glm::vec3(0, 1, 0));

    material->setUniform("color", glm::vec3(1, 0.4, 0));

    auto geometry = dynamic_pointer_cast<BufferGeometry>(make_shared<BoxBufferGeometry>());

    scene->add(unique_ptr<Object3D>(new Mesh(geometry, material)));

    const auto camera = make_unique<Camera>(make_unique<PerspectiveProjectionData>(16.f / 9.f));

    const auto window = renderer->getWindow()->getGlfwWindow();
    using namespace std::chrono;
    uint64_t start = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    double t;
    while (!glfwWindowShouldClose(window)) {

        uint64_t uit = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
        t = (uit - start);
        t /= 1000.0;

//        material->getShader()->debugPrintActiveUniforms();

        auto newPos = glm::vec3(2.*glm::cos(t), 1, 2.*glm::sin(t));
        camera->setPosition(newPos);
        camera->lookAt();

        renderer->render(scene.get(), camera.get());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}