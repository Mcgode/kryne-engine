//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/3DObjects/Object3D.h"


Object3D::Object3D()
{
    this->id = Object3D::getId();
    this->scale = glm::vec3(1.f, 1.f, 1.f);
}


uint32_t Object3D::nextId()
{
    // Use static atomic to handle next id thread-safely
    static std::atomic<uint32_t> uid { 0 };
    return ++uid;
}


void Object3D::update(bool force)
{
    if ((this->matrixWorldNeedsUpdate || force) && this->visible)
    {
        this->localTransform = glm::identity<glm::mat4>();
        this->localTransform = glm::scale(this->localTransform, this->scale);
        this->localTransform = glm::mat4_cast(this->quaternion) * this->localTransform;
        this->localTransform = glm::translate(this->localTransform, this->position);

        this->matrixWorld = this->parent != nullptr ?
                this->parent->matrixWorld * this->localTransform :
                this->localTransform;

        for (const auto& child : this->children)
            child->update(true);

        this->matrixWorldNeedsUpdate = false;
    }
}


void Object3D::add(unique_ptr<Object3D> child)
{
    if (child->parent != nullptr)
        child->parent->remove(child.get());

    child->parent = this;
    this->children.push_back(std::move(child));
}


unique_ptr<Object3D> Object3D::remove(Object3D *childToRemove)
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


void Object3D::traverse(Object3D::TraverseCallback callback)
{
    callback(this);
    for (const auto &child : this->children)
        child->traverse(callback);
}


vector<Object3D *> Object3D::getChildren() {
    vector<Object3D *> result;
    for (const auto &child : this->children)
        result.push_back(child.get());
    return result;
}


Object3D::~Object3D()
{
    for (const auto& child : this->children) {
        child->parent = nullptr;
    }
}


void Object3D::lookAt(const glm::vec3 &target, const glm::vec3 &up)
{
    auto p = getWorldPosition();
    auto d = target - p;

    if (glm::length2(d) == 0.f)
        d.z = 1.f;

    d = glm::normalize(d);
    auto x = glm::cross(up, d);

    if (glm::length2(x) == 0.f) {
        if (glm::abs(d.z) == 1.0f)
            d.x += 0.0001f;
        else
            d.z += 0.0001f;

        d = glm::normalize(d);
    }

    this->setQuaternion(glm::quat_cast(glm::lookAt(d + p, p, up)));
}


const glm::mat4 &Object3D::updateParents(const Object3D *caller)
{
    if (this->matrixWorldNeedsUpdate)
        this->calculateLocalTransform();

    this->matrixWorld = this->parent != nullptr ?
                        this->parent->updateParents(this) * this->localTransform :
                        this->localTransform;

    for (const auto &child : this->children) {
        if (child.get() != caller)
            child->setWorldMatrixNeedsUpdate();
    }

    return this->matrixWorld;
}
