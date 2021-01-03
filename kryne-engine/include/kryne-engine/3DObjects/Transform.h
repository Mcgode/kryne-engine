/**
 * @file
 * @author Max Godefroy
 * @date 21/11/2020
 */

#ifndef INC_KRYNE_ENGINE_OBJECT3D_H
#define INC_KRYNE_ENGINE_OBJECT3D_H


// Circular dependencies forward declaration
class Entity;


#include <memory>
#include <vector>

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <kryne-engine/Math/Transform.hpp>


using namespace std;

/**
 * An object in 3D space
 */
class Transform {


// -- Base --

public:

    /**
     * Initializes the transforms and saves its attached entity.
     */
    explicit Transform(Entity *entity);

    /**
     * Object3D destructor.
     * All its children's #parent value will be set to `nullptr`, and it will be removed from its parent (if it has
     * one).
     */
    virtual ~Transform();

    /**
     * Returns the entity this transform is attached to
     * @return A raw pointer to the attached entity.
     */
    [[nodiscard]] const Entity *getEntity() const { return attachedEntity; }

protected:

    /// The entity this transform is attached to.
    const Entity *attachedEntity;


// -- Hierarchy --

public:

    /**
     * The traversal callback function type.
     */
    typedef void (*TraverseCallback)(Transform *object);

    /**
     * Adds a child to the object. If the new child already has a parent, call #remove() first.
     * @param child The new child to add.
     */
    void add(Transform *child);

    /**
     * Removes the provided object from this object's children, if it belonged to it.
     * @param childToRemove The raw pointer to the child to remove.
     * @return `true` is the provided transform was removed, `false` otherwise
     */
    bool remove(Transform *childToRemove);

    /**
     * Removes this object from its parent, if it has one.
     */
    void removeFromParent() { if (this->parent) this->parent->remove(this); }

    /**
     * Retrieve the object's parent
     * @return A raw pointer to the object's parent. May be null.
     */
    [[nodiscard]] Transform *getParent() const { return parent; }

    /**
     *
     * @param callback
     */
    void traverse(TraverseCallback callback);

    /**
     * Retrieve the object's children
     * @return A list of raw pointers to the children
     */
    [[nodiscard]] const vector<Transform *> &getChildren() const { return children; }

protected:

    /// The parent of the object
    Transform *parent = nullptr;

    /// The object children
    vector<Transform *> children {};


// -- Transform --

/**
 * Replace the needs update value check according to the defined behaviour
 */
#ifndef NO_CHANGE_CHECK
#define needsUpdate(a,b) !glm::all(glm::equal(a,b))
#else
#define needsUpdate(a,b) true
#endif


public:

    /**
     * Updates the object transform if needed.
     * @param force  Set to true to force the update
     */
    virtual void updateTransform(bool force);

    /**
     * Update the object itself along its parents.
     * @param caller  The child that called the object. Can be null
     * @return The new matrixWorld value of this object
     */
    virtual const glm::mat4 &updateParents(const Transform *caller);

    /**
     * Retrieve the current object position.
     */
    [[nodiscard]] const glm::vec3 &getPosition() const { return position; }

    /**
     * Change the object position
     * @param pos   The new position value
     */
    virtual void setPosition(const glm::vec3 &pos) {
        matrixWorldNeedsUpdate = needsUpdate(pos, Transform::position);
        Transform::position = glm::vec3(pos);
    }

    /**
     * Retrieve the current object quaternion.
     */
    [[nodiscard]] const glm::quat &getQuaternion() const { return quaternion; }

    /**
     * Change the object quaternion.
     * @param quat  The new quaternion value.
     */
    void setQuaternion(const glm::quat &quat) {
        matrixWorldNeedsUpdate = needsUpdate(quat, Transform::quaternion);
        Transform::quaternion = glm::quat(quat);
    }

    /**
     * Retrieve the current object scale.
     */
    [[nodiscard]] const glm::vec3 &getScale() const { return scale; }

    /**
     * Changes the object scale.
     * @param s The new scale information.
     */
    void setScale(const glm::vec3 &s) {
        matrixWorldNeedsUpdate = needsUpdate(s, Transform::scale);
        Transform::scale = glm::vec3(s);
    }

    /**
     * Marks the object as needing a matrix world update.
     */
    void setWorldMatrixNeedsUpdate() { this->matrixWorldNeedsUpdate = true; }

    /**
     * Retrieve the normal matrix for this object
     */
    [[nodiscard]] const mat3 &getNormalMatrix() const { return this->normalMatrix; }

protected:

    /**
     * Computes the local transform matrix.
     */
    void calculateLocalTransform();

protected:

    /// The local transform matrix.
    glm::mat4 localTransform {};

    /// The object world matrix.
    glm::mat4 matrixWorld {};

    /// The world normal matrix of the object.
    glm::mat3 normalMatrix {};

    /// Set to true to update the world matrix at the next #updateTransform() call.
    bool matrixWorldNeedsUpdate = true;

    /// The local position of the object.
    glm::vec3 position {};

    /// The local rotation quaternion.
    glm::quat quaternion {};

    /// The local object scale.
    glm::vec3 scale {};


public:

    /**
     * Retrieves the world position of the object
     * @return A `vec3` of the object's world position.
     */
    glm::vec3 getWorldPosition();

    /**
     * Changes the object's rotation (through #quaternion) for it to look at the target position.
     * @param target    The target position in world coordinates.
     * @param up        The up direction in world coordinates.
     */
    virtual void lookAt(const glm::vec3 &target = glm::vec3(0), const glm::vec3 &up = glm::vec3(0, 1, 0)) {
        this->applyLookAt(this->getWorldPosition(), target, up);
    }

protected:

    /**
     * Computes and applies the lookAt transformation.
     * @param eye       The eye position.
     * @param target    The target position, in the same coordinate space.
     * @param up        The up vector, in the same coordinate space.
     */
    inline void applyLookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up);

};



// Circular dependencies includes
#include <kryne-engine/Core/Entity.h>


#endif //INC_KRYNE_ENGINE_OBJECT3D_H
