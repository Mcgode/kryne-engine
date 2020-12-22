/**
 * @file
 * @author Max Godefroy
 * @date 21/11/2020
 */

#ifndef INC_KRYNE_ENGINE_OBJECT3D_H
#define INC_KRYNE_ENGINE_OBJECT3D_H




#include <memory>
#include <atomic>
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
class Object3D {


// -- Base --

public:

    /**
     * Initializes the object and assigns it its id.
     */
    Object3D();

    /**
     * Retrieve the unique id of this object
     */
    [[nodiscard]] uint32_t getId() const { return id; }

protected:

    /**
     * Retrieves the unique id for the next instance of #Object3D.
     * Thread-safe operation, thanks to an atomic counter.
     * @return A unique id.
     */
    static uint32_t nextId();

protected:

    /// The unique id of the object.
    uint32_t id;


// -- Rendering --

public:

    /**
     * Retrieves the value of #visible
     */
    [[nodiscard]] bool isVisible() const { return visible; }

    /**
     * Set the value of #visible.
     * @param v The new value of #visible.
     */
    void setVisible(bool v) { Object3D::visible = v; }

    /**
     * Object3D destructor.
     * All its children's #parent value will be set to `nullptr`.
     */
    virtual ~Object3D();

protected:

    /// If set to false, the object won't be updated and won't be rendered.
    bool visible = true;


// -- Hierarchy --

public:

    /**
     * The traversal callback function type.
     */
    typedef void (*TraverseCallback)(Object3D *object);

    /**
     * Adds a child to the object. If the new child already has a parent, call #remove() first.
     * @param child The new child to add.
     */
    void add(unique_ptr<Object3D> child);

    /**
     * Removes the provided object from this object's children, if it belonged to it.
     * @param childToRemove The raw pointer to the child to remove.
     * @return The unique pointer to the removed child. May be null.
     */
    unique_ptr<Object3D> remove(Object3D *childToRemove);

    /**
     * Removes this object from its parent, if it has one.
     * @return The unique pointer for this object. Will be null if the object has no parent.
     */
    unique_ptr<Object3D> removeFromParent() { return this->parent ? this->parent->remove(this) : nullptr; }

    /**
     * Retrieve the object's parent
     * @return A raw pointer to the object's parent. May be null.
     */
    [[nodiscard]] Object3D *getParent() const { return parent; }

    /**
     *
     * @param callback
     */
    void traverse(TraverseCallback callback);

    /**
     * Retrieve the object's children
     * @return A list of raw pointers to the children
     */
    vector<Object3D *> getChildren();

protected:

    /// The parent of the object
    Object3D *parent = nullptr;

    /// The object children
    vector<unique_ptr<Object3D>> children {};


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
    virtual const glm::mat4 &updateParents(const Object3D *caller);

    /**
     * Retrieve the current object position.
     */
    [[nodiscard]] const glm::vec3 &getPosition() const { return position; }

    /**
     * Change the object position
     * @param pos   The new position value
     */
    virtual void setPosition(const glm::vec3 &pos) {
        matrixWorldNeedsUpdate = needsUpdate(pos, Object3D::position);
        Object3D::position = glm::vec3(pos);
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
        matrixWorldNeedsUpdate = needsUpdate(quat, Object3D::quaternion);
        Object3D::quaternion = glm::quat(quat);
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
        matrixWorldNeedsUpdate = needsUpdate(s, Object3D::scale);
        Object3D::scale = glm::vec3(s);
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


#endif //INC_KRYNE_ENGINE_OBJECT3D_H
