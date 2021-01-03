//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/3DObjects/Mesh.h"


void Mesh::updateTransform(bool force)
{
    const bool willUpdate = (this->matrixWorldNeedsUpdate || force) && this->visible;
    Transform::updateTransform(force);
    if (willUpdate) {
        this->material->setUniform("modelMatrix", this->matrixWorld);
        this->material->setUniform("normalMatrix", this->normalMatrix);
    }
}


const mat4 &Mesh::updateParents(const Transform *caller)
{
    const auto &matrixWorld = Transform::updateParents(caller);
    this->material->setUniform("modelMatrix", matrixWorld);
    this->material->setUniform("normalMatrix", normalMatrix);
    return matrixWorld;
}
