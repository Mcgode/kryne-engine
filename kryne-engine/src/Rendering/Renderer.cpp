//
// Created by Max Godefroy on 20/11/2020.
//

#include "kryne-engine/Rendering/Renderer.h"

Renderer::Renderer(uint32_t width, uint32_t height)
{
    this->associatedWindow = make_unique<Window>(width, height);
}


void Renderer::render(Scene *scene, Camera *camera)
{
    scene->traverse(&updateObjects);

    this->renderObject(scene, camera);
}


void Renderer::updateObjects(Object3D *object)
{
    object->update();
}


void Renderer::renderObject(Object3D *object, Camera *camera)
{
    if (!object->isVisible())
        return;

    for (auto child: object->getChildren())
        renderObject(child, camera);
}
