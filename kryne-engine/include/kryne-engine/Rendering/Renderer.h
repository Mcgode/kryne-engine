//
// Created by Max Godefroy on 20/11/2020.
//

#ifndef INC_3D_DEMOS_RENDERER_H
#define INC_3D_DEMOS_RENDERER_H

#include <memory>
#include <kryne-engine/Core/Window.h>
#include <kryne-engine/3DObjects/Scene.h>
#include <kryne-engine/3DObjects/Mesh.h>

using namespace std;


class Renderer {

public:

    Renderer(uint16_t width, uint16_t height);

    void render(Scene *scene, Camera *camera);

    [[nodiscard]] const Window *getWindow() {
        return Renderer::associatedWindow.get();
    }

private:

    unique_ptr<Window> associatedWindow;

    void renderObject(Object3D *object, Camera *camera);

    static void updateObjects(Object3D *object);

};


#endif //INC_3D_DEMOS_RENDERER_H
