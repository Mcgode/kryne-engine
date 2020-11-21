//
// Created by Max Godefroy on 20/11/2020.
//

#ifndef INC_3D_DEMOS_RENDERER_H
#define INC_3D_DEMOS_RENDERER_H

#include <memory>
#include <kryne-engine/Core/Window.h>
#include <kryne-engine/3DObjects/Scene.h>

using namespace std;


class Renderer {

public:

    Renderer(uint32_t width, uint32_t height);

    void render(Scene *scene, Camera *camera);

private:

    unique_ptr<Window> associatedWindow;

    void renderObject(Object3D *object, Camera *camera);

    static void updateObjects(Object3D *object);

};


#endif //INC_3D_DEMOS_RENDERER_H
