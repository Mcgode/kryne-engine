//
// Created by Max Godefroy on 20/11/2020.
//

#include "kryne-engine/Rendering/Renderer.h"

Renderer::Renderer(uint16_t width, uint16_t height) :
    renderingStatus(FrontSide)
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

    auto mesh = dynamic_cast<Mesh *>(object);
    if (mesh) {
        const auto& material = mesh->getMaterial();

        material->use();

        auto side = material->getSide();
        bool differentSides = side != this->renderingStatus.getSide();
        if (differentSides)
            renderingStatus.setSide(side);

        mesh->onBeforeRender(camera);

        mesh->getGeometry()->draw(material->getPrimitiveType());

        material->resetUse();
    }

    for (auto child: object->getChildren())
        renderObject(child, camera);
}
