//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/3DObjects/Transform.h"


Transform::Transform()
{
    this->scale = glm::vec3(1.f, 1.f, 1.f);
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
    if ((this->matrixWorldNeedsUpdate || force) && this->visible)
    {
        calculateLocalTransform();

        this->matrixWorld = this->parent != nullptr ?
                this->parent->matrixWorld * this->localTransform :
                this->localTransform;
        this->normalMatrix = mat3(transpose(inverse(this->matrixWorld)));

        for (const auto& child : this->children)
            child->updateTransform(true);

        this->matrixWorldNeedsUpdate = false;
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

    for (const auto &child : this->children) {
        if (child.get() != caller)
            child->setWorldMatrixNeedsUpdate();
    }

    return this->matrixWorld;
}


void Transform::add(unique_ptr<Transform> child)
{
    if (child->parent != nullptr)
        child->parent->remove(child.get());

    child->parent = this;
    this->children.push_back(std::move(child));
}


unique_ptr<Transform> Transform::remove(Transform *childToRemove)
{
    for (auto it = this->children.begin(); it != this->children.end(); it++) {
        if (it->get() == childToRemove) {
            auto result = std::move(*it);
            this->children.erase(it);
            return result;
        }
    }
    return nullptr;
}


void Transform::traverse(Transform::TraverseCallback callback)
{
    callback(this);
    for (const auto &child : this->children)
        child->traverse(callback);
}


vector<Transform *> Transform::getChildren() {
    vector<Transform *> result;
    for (const auto &child : this->children)
        result.push_back(child.get());
    return result;
}


Transform::~Transform()
{
    for (const auto& child : this->children) {
        child->parent = nullptr;
    }
}


glm::vec3 Transform::getWorldPosition() {
    if (this->parent == nullptr) {
        return this->position;
    } else {
        this->updateParents(nullptr);
        auto p = this->parent->matrixWorld * glm::vec4(position, 1.);
        return glm::vec3(p) * (1.f / p.w);
    }
}


void Transform::applyLookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
{
    this->setQuaternion(glm::toQuat(Math::getLookAtMatrix(eye, target, up)));
}
