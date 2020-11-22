//
// Created by Max Godefroy on 22/11/2020.
//

#include <kryne-engine/Rendering/Renderer.h>
#include <kryne-engine/Geometry/BoxBufferGeometry.h>

int main()
{
    auto renderer = make_unique<Renderer>(1280, 720);

    auto scene = make_unique<Scene>();

    const string vertexShader = "#version 330 core\n"
                                "layout (location = 0) in vec3 position;\n"
                                "uniform mat4 modelMatrix;\n"
                                "\n"
                                "void main() {\n"
                                "   gl_Position = modelMatrix * vec4(position, 1.);\n"
                                "}";

    const string fragmentShader =   "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "\n"
                                    "void main() {\n"
                                    "   FragColor = vec4(0.8, 0.4, 0.1, 1.0);\n"
                                    "}";

    auto material = make_shared<Material>(make_unique<Shader>(vertexShader, fragmentShader));
    auto geometry = dynamic_pointer_cast<BufferGeometry>(make_shared<BoxBufferGeometry>());

    scene->add(unique_ptr<Object3D>(new Mesh(geometry, material)));

    const auto window = renderer->getWindow()->getGlfwWindow();
    while (!glfwWindowShouldClose(window)) {
        renderer->render(scene.get(), nullptr);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}