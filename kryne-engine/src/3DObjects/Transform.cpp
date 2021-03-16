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


void Transform::updateTransform(bool force, bool updateChildren)
{
    if ((this->matrixWorldNeedsUpdate || force) && this->attachedEntity->isEnabled())
    {
        calculateLocalTransform();

        this->matrixWorld = this->parent != nullptr ?
                this->parent->matrixWorld * this->localTransform :
                this->localTransform;
        this->normalMatrix = mat3(transpose(inverse(this->matrixWorld)));

        for (const auto& child : this->children)
        {
            if (updateChildren)
                child->updateTransform(true, updateChildren);
            else
                child->setWorldMatrixNeedsUpdate();
        }

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

    if (child->attachedScene != this->attachedScene) {

        if (child->attachedScene)
            child->attachedScene->remove(child);
        this->attachedScene->add(child);

    }
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
    for (const auto& child : this->children)
        child->parent = nullptr;

    this->removeFromParent();

    if (this->attachedScene != nullptr)
        this->attachedScene->remove(this);
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
        return vec3(p) * (1.f / p.w);
    }
}


void Transform::lookAt(const vec3 &target, const vec3 &up, bool swap)
{
    if ( swap )
        this->setQuaternion(toQuat(Math::getLookAtMatrix(target, position, up)));

    else
        this->setQuaternion(toQuat(Math::getLookAtMatrix(position, target, up)));
}


void Transform::setScene(Scene *scene)
{
    if (scene != nullptr)
        scene->add(this);
    else if (this->attachedScene != nullptr)
        this->attachedScene->remove(this);
}
