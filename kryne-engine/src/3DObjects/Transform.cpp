//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/3DObjects/Transform.h"


Transform::Transform(Entity *entity) :
    attachedEntity(entity)
{
    this->scale = glm::vec3(1);
}


void Transform::calculateLocalTransform()
{
    const auto scaleMatrix = glm::scale(glm::mat4(1), this->scale);
    const auto rotationMatrix = glm::toMat4(this->quaternion);
    const auto translationMatrix = glm::translate(glm::mat4(1), this->position);
    this->localTransform = translationMatrix * rotationMatrix * scaleMatrix;
}


void Transform::updateTransform(bool force)
{
    if ((this->matrixWorldNeedsUpdate || force) && this->attachedEntity->isEnabled())
    {
        calculateLocalTransform();

        this->matrixWorld = this->parent != nullptr ?
                this->parent->matrixWorld * this->localTransform :
                this->localTransform;
        this->normalMatrix = mat3(transpose(inverse(this->matrixWorld)));

        for (const auto& child : this->children)
            child->updateTransform(true);

        this->matrixWorldNeedsUpdate = false;

        this->attachedEntity->transformDidUpdate();
    }
}


const glm::mat4 &Transform::updateParents(const Transform *caller)
{
    if (this->matrixWorldNeedsUpdate)
        this->calculateLocalTransform();

    this->matrixWorld = this->parent != nullptr ?
                        this->parent->updateParents(this) * this->localTransform :
                        this->localTransform;
    this->normalMatrix = mat3(transpose(inverse(this->matrixWorld)));

    for (const auto &child : this->children)
    {
        if (child != caller)
            child->setWorldMatrixNeedsUpdate();
    }

    return this->matrixWorld;
}


void Transform::add(Transform *child)
{
    child->removeFromParent();

    child->parent = this;
    this->children.push_back(child);
}


bool Transform::remove(Transform *childToRemove)
{
    for (auto it = this->children.begin(); it != this->children.end(); it++)
    {
        if (*it == childToRemove)
        {
            this->children.erase(it);
            return true;
        }
    }
    return false;
}


void Transform::traverse(Transform::TraverseCallback callback)
{
    callback(this);
    for (const auto &child : this->children)
        child->traverse(callback);
}


Transform::~Transform()
{
    for (const auto& child : this->children) {
        child->parent = nullptr;
    }
    this->removeFromParent();
}


glm::vec3 Transform::getWorldPosition()
{
    if (this->parent == nullptr)
    {
        return this->position;
    }
    else
    {
        this->updateParents(nullptr);
        auto p = this->parent->matrixWorld * glm::vec4(position, 1.);
        return glm::vec3(p) * (1.f / p.w);
    }
}


void Transform::applyLookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
{
    this->setQuaternion(glm::toQuat(Math::getLookAtMatrix(eye, target, up)));
}
