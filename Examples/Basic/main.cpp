//
// Created by Max Godefroy on 22/11/2020.
//

#include <kryne-engine/Camera/PerspectiveProjectionData.hpp>
#include <kryne-engine/Rendering/Renderer.h>
#include <kryne-engine/Geometry/BoxBufferGeometry.h>

int main()
{
    auto renderer = make_unique<Renderer>(1280, 720);

    auto scene = make_unique<Scene>();

    const string vertexShader = "layout (location = 0) in vec3 position;\n"
                                "layout (location = 1) in vec3 normal;\n"
                                "uniform mat4 projectionMatrix;\n"
                                "uniform mat4 viewMatrix;\n"
                                "uniform mat4 modelMatrix;\n"
                                "out vec3 vNormal;\n"
                                "\n"
                                "void main() {\n"
                                "   vNormal = normal;\n"
                                "   vec3 transformed = position;\n"
                                "   #include <vertex_position>;\n"
                                "}";

    const string fragmentShader =   "in vec3 vNormal;\n"
                                    "out vec4 FragColor;\n"
                                    "\n"
                                    "void main() {\n"
                                    "   #ifndef TEST\n"
                                    "       lmlmkmkmk;\n"
                                    "   #endif\n"
                                    "   FragColor = vec4(0.5 * normalize(vNormal) + 0.5, 1.0);\n"
                                    "}";

    auto material = make_shared<Material>(make_unique<Shader>(vertexShader, fragmentShader));
    material->setSide(BackSide);
    material->setDefine("TEST", "");
    auto geometry = dynamic_pointer_cast<BufferGeometry>(make_shared<BoxBufferGeometry>());

    scene->add(unique_ptr<Object3D>(new Mesh(geometry, material)));

    const auto camera = make_unique<Camera>(make_unique<PerspectiveProjectionData>(16.f / 9.f));

    const auto window = renderer->getWindow()->getGlfwWindow();
    float t = 0.f;
    int i = 0;
    while (!glfwWindowShouldClose(window)) {

        auto newPos = glm::vec3(2.*glm::cos(t), 0, 2.*glm::sin(t));
        camera->setPosition(newPos);
        camera->lookAt();

        renderer->render(scene.get(), camera.get());
        glfwSwapBuffers(window);
        glfwPollEvents();

        i++;
        t += 0.01f;
    }

    return 0;
}