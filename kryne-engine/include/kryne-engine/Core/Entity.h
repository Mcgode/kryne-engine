/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#ifndef KRYNE_ENGINE_ENTITY_H
#define KRYNE_ENGINE_ENTITY_H


// Circular dependencies forward declaration
class Process;
//class Component;
//class Transform; <- class is guaranteed to be pre-declared


#include <atomic>
#include <vector>
#include <unordered_set>
#include <mutex>
#include <imgui.h>

// Pre-declared circular dependencies includes
#include <kryne-engine/3DObjects/Transform.h>
#include "Component.h"


using namespace std;


class Entity {

// -----
// Base
// -----


public:

    /**
     * Initializes the object and assigns it its id.
     * @param process   The process the entity is attached to.
     */
    explicit Entity(Process *process);

    /**
     * Retrieve the unique id of this object
     */
    [[nodiscard]] uint32_t getId() const { return id; }

    /**
     * Retrieves the process this entity is attached to.
     * @return The process this instance is attached to.
     */
    [[nodiscard]] inline Process *getProcess() const { return process; }

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

private:

    /// The process this entity is attached to.
    Process *process;


// -----
// Name
// -----


public:

    [[nodiscard]] const string &getName() const { return name; }

    void setName(const string &newName) { Entity::name = newName; }

protected:

    string name = "Entity";


// -----
// DearImGui
// -----


public:

    /**
     * @brief Renders custom GUI for entity details.
     */
    virtual void renderEntityDetails() {}


// -----
// Transform
// -----


public:

    /**
     * Retrieves the transform of this entity.
     * @return A raw pointer to this entity's 3D transform.
     */
    [[nodiscard]] inline Transform *getTransform() { return this->transform.get(); }

    /**
     * A callback that is called if the entity's transform was updated.
     */
    virtual void transformDidUpdate();


protected:

    /// The 3D transform for this entity
    unique_ptr<Transform> transform;


// -----
// Enabling
// -----

public:

    /**
     * Retrieves the value of #enabled
     * @return
     */
    [[nodiscard]] inline bool isEnabled() const { return enabled; }

    /**
     * Enables/disables the entity.
     * When disabled, an entity won't be updated or rendered.
     * @param newValue Set to `true` to enable, `false` to disable.
     */
    void setEnabled(bool newValue) { Entity::enabled = newValue; }

protected:

    /// Enabled status of the entity.
    bool enabled = true;


// -----
// Components
// -----


public:

    /**
     * @brief Initializes and attaches a new component to the entity.
     * @tparam T        The component class
     * @tparam Args     The class constructor argument types collection.
     * @param args      The arguments for the class constructor
     * @return A raw pointer to the new component
     *
     * @details The function will call the corresponding component constructor, by prepending a pointer to itself to the
     *          argument list. As such, the called constructor must be in the form `Component(Entity *, Args ...args)`.
     */
    template<typename T, typename... Args>
    T *addComponent(Args&&... args)
    {
        static_assert(is_convertible_v<T*, Component*>, "Class must inherit from Component");

        const auto component = make_shared<T>(this, forward<Args>(args)...);
        this->components.emplace(component);
        return component.get();
    }

    /**
     * Retrieves the first component belonging to the provided class.
     * @tparam C    The component class. Used to find it using dynamic_cast.
     * @return A raw pointer to the component. If none were found, will be `nullptr`.
     */
    template<class C>
    C *getComponent() const
    {
        for (const auto & it : this->components)
        {
            auto component = dynamic_cast<C *>(it.get());
            if (component)
                return component;
        }
        return nullptr;
    }

    /**
     * @brief Retrieves components belonging to the provided class.
     *
     * @tparam C    The component class. Used to find the components using dynamic_cast.
     *
     * @param  mustBeEnabled    If true, will only retrieve enabled components.
     *
     * @return A vector of all the components.
     */
    template<class C>
    vector<C *> getComponents(bool mustBeEnabled = false) const
    {
        vector<C *> result {};

        for (const auto &it : this->components)
        {
            const auto component = dynamic_cast<C *>(it.get());
            if (component && (!mustBeEnabled || it->isEnabled()))
                result.push_back(component);
        }

        return result;
    }


    vector<Component *> getAllComponents();

private:

    /// The components attached to this entity.
    unordered_set<shared_ptr<Component>> components {};


// -----
// Pre-processing
// -----


friend class Process;
friend class Scene;
friend struct ProcessCommon;


public:

    /**
     * @brief Sets an entity that needs to be preprocessed before preprocessing this entity in an unsafe way.
     *
     * @details
     * This property will ensure that the provided entity will be pre-processed before the current entity. <br>
     * To be used in moderation, as it might increase execution time or lead to deadlock situations.
     *
     * @warning
     * You need to update this manually, and reset the value when needed, to avoid segmentation faults. <br>
     * You should also be very careful with its use. If not, you might create a dependency cycle and get locked in an
     * infinite loop.
     *
     * @param e     The entity that needs to have already been preprocessed.
     */
    void unsafeSetPreprocessingRequirement(Entity *e) { this->preprocessingRequirement = e; }

private:

    mutex preRenderingProcessingMutex {};

    bool ranPreRenderingProcessing = false;

    /// An entity that needs to have already been preprocessed.
    Entity *preprocessingRequirement = nullptr;

};


// Circular dependencies includes
#include "Process.h"


#endif //KRYNE_ENGINE_ENTITY_H
