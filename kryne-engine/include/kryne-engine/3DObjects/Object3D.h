//
// Created by Max Godefroy on 21/11/2020.
//

#ifndef INC_3D_DEMOS_OBJECT3D_H
#define INC_3D_DEMOS_OBJECT3D_H




#include <memory>
#include <atomic>
#include <vector>

#define GLM_FORCE_SWIZZLE
#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


using namespace std;

class Object3D {


// -- Base --

public:
    Object3D();

    [[nodiscard]] uint32_t getId() const { return id; }

protected:

    uint32_t id;

    static uint32_t nextId();


// -- Rendering --

public:

    [[nodiscard]]
    bool isVisible() const {
        return visible;
    }

    void setVisible(bool v) {
        Object3D::visible = v;
    }

    virtual ~Object3D();

protected:

    bool visible = true;


// -- Hierarchy --

typedef void (*TraverseCallback)(Object3D *object);

public:

    void add(unique_ptr<Object3D> child);

    unique_ptr<Object3D> remove(Object3D *childToRemove);

    unique_ptr<Object3D> removeFromParent() { return this->parent->remove(this); }

    [[nodiscard]]
    Object3D *getParent() const {
        return parent;
    }

    void traverse(TraverseCallback callback);

    vector<Object3D *> getChildren();

protected:

    Object3D *parent = nullptr;

    vector<unique_ptr<Object3D>> children {};


// -- Transform --

#ifndef NO_CHANGE_CHECK
#define needsUpdate(a,b) !glm::all(glm::equal(a,b))
#else
#define needsUpdate(a,b) true
#endif


public:

    virtual void update(bool force);

    virtual const glm::mat4 &updateParents(const Object3D *caller);

    [[nodiscard]]
    const glm::vec3 &getPosition() const {
        return position;
    }

    void setPosition(const glm::vec3 &pos) {
        matrixWorldNeedsUpdate = needsUpdate(pos, Object3D::position);
        Object3D::position = pos;
    }

    [[nodiscard]]
    const glm::quat &getQuaternion() const {
        return quaternion;
    }

    void setQuaternion(const glm::quat &quat) {
        matrixWorldNeedsUpdate = needsUpdate(quat, Object3D::quaternion);
        Object3D::quaternion = quat;
    }

    [[nodiscard]]
    const glm::vec3 &getScale() const {
        return scale;
    }

    void setScale(const glm::vec3 &s) {
        matrixWorldNeedsUpdate = needsUpdate(s, Object3D::scale);
        Object3D::scale = s;
    }

    void setWorldMatrixNeedsUpdate() {
        matrixWorldNeedsUpdate = true;
    }

protected:

    void calculateLocalTransform();

    glm::mat4 localTransform {};

    glm::mat4 matrixWorld {};

    bool matrixWorldNeedsUpdate = true;

    glm::vec3 position {};

    glm::quat quaternion {};

    glm::vec3 scale {};


public:

    glm::vec3 getWorldPosition() {
        auto p = matrixWorld * glm::vec4(position, 1.);
        return p.xyz * (1.f / p.w);
    }

    void lookAt(const glm::vec3 &target = glm::vec3(0), const glm::vec3 &up = glm::vec3(0, 1, 0));

};


#endif //INC_3D_DEMOS_OBJECT3D_H
