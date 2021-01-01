/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#ifndef KRYNE_ENGINE_ENTITY_H
#define KRYNE_ENGINE_ENTITY_H


class Process;
class Component;


#include <atomic>
#include <vector>
#include <unordered_set>
#include "Process.h"
#include "Component.h"


using namespace std;


class Entity {

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

protected:

    /**
     * Retrieves the unique id for the next instance of #Object3D.
     * Thread-safe operation, thanks to an atomic counter.
     * @return A unique id.
     */
    static uint32_t nextId();

    /**
     * Retrieves the process this entity is attached to.
     * @return The process this instance is attached to.
     */
    [[nodiscard]] inline Process *getProcess() const { return process; }

protected:

    /// The unique id of the object.
    uint32_t id;

private:

    /// The process this entity is attached to.
    Process *process;

public:

    /**
     * Initializes and attaches a new component to the entity.
     * @tparam T        The component class
     * @tparam Args     The class constructor argument types collection.
     * @param args      The arguments for the class constructor
     * @return A raw pointer to the new component
     */
    template<typename T, typename... Args>
    T *addComponent(Args&&... args);

    /**
     * Retrieves the first component belonging to the provided class.
     * @tparam C    The component class. Used to find it using dynamic_cast.
     * @return A raw pointer to the component. If none were found, will be `nullptr`.
     */
    template<class C>
    C *getComponent();

    /**
     * Retrieves components belonging to the provided class.
     * @tparam C    The component class. Used to find the components using dynamic_cast.
     * @return A vector of all the components.
     */
    template<class C>
    vector<C *> getComponents();

private:

    /// The components attached to this entity.
    unordered_set<shared_ptr<Component>> components {};

};


#endif //KRYNE_ENGINE_ENTITY_H
