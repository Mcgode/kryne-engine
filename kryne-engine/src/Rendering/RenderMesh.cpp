/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#include "kryne-engine/Rendering/RenderMesh.h"


RenderMesh::RenderMesh(Entity *entity, shared_ptr<BufferGeometry> geometry, shared_ptr<Material> material):
    Component(entity),
    geometry(move(geometry)),
    material(move(material))
{
    this->componentName = "RenderMesh";
}


void RenderMesh::transformDidUpdate()
{
    auto transform = this->getEntity()->getTransform();
    if (this->geometry != nullptr)
        this->boundingSphere = this->geometry->getBoundingSphere().fromMatrix(transform->getWorldMatrix());
    else
        this->boundingSphere = Math::Sphere();
}
