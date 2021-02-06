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
{}
