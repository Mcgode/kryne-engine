/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#ifndef KRYNE_ENGINE_PROCESS_H
#define KRYNE_ENGINE_PROCESS_H


#include <memory>
#include <unordered_map>
#include <iostream>

#include "Entity.h"
#include "System.h"
#include "GraphicContext/GraphicContext.h"


using namespace std;



// ===========
// Core process logic
// ===========

class Process {


public:

    Process(GraphicContext *context): context(context) {}

    /**
     * Instantiates a new scene.
     */
    Scene *makeScene();

    /**
     * Runs a single game loop
     */
    void runLoop();

    /**
     * Returns the graphic context used for this process.
     */
    GraphicContext *getGraphicContext() const { return this->context; }

    /**
     * Set the value of #currentScene
     * @param scene
     */
    void setCurrentScene(Scene *scene) { this->currentScene = scene; }

protected:

    /// The graphical context for this process
    GraphicContext *context;

    /// The current scene used in the loop
    Scene *currentScene = nullptr;

    /// The scenes for this process
    unordered_set<unique_ptr<Scene>> scenes;


// ===========
// Entities-related interface
// ===========

public:

    /**
     * Initializes an entity and attaches it to this process.
     * @tparam T        The entity class
     * @tparam Args     The class constructor argument types collection.
     * @param args      The arguments for the class constructor
     * @return A pointer to the newly created entity. The pointer is linked to a shared pointer owned only by the
     *         process. Weak reference to the object can be retrieved using #getWeakReference
     */
    template<typename T, typename... Args>
    inline T *makeEntity(Args&&... args)
    {
        static_assert(is_convertible<T, Entity>::value, "Class must inherit from Entity");

        const auto entity = make_shared<T>(this, forward<Args>(args)...);
        this->processEntities.emplace(pair(entity.get(), entity));
        return entity.get();
    }

    /**
     * Retrieves a weak reference of an entity attached to the process.
     * @param system    The attached entity.
     * @return  A weak reference to the provided entity. Will link to nullptr if the provided entity is not attached to
     *          the process.
     */
    weak_ptr<Entity> getWeakReference(Entity *entity);

    /**
     * Detaches an entity from the process, meaning it should be deleted (if there is no other shared reference active
     * at the moment).
     * @param entity    The entity to detach.
     * @return `true` if the provided entity was attached. `false` otherwise.
     */
    bool detachEntity(Entity *entity);

protected:

    /// The set of entities attached to this process, mapping the pointer to its corresponding shared pointer.
    unordered_map<Entity *, shared_ptr<Entity>> processEntities;


// ===========
// Systems-related interface
// ===========

public:

    /**
     * Initializes a system and attaches it to this process.
     * @tparam T        The system class
     * @tparam Args     The class constructor argument types collection.
     * @param args      The arguments for the class constructor
     * @return A pointer to the newly created system. The pointer is linked to a shared pointer owned only by the
     *         process. Weak reference to the object can be retrieved using #getWeakReference
     */
    template<typename T, typename... Args>
    T *makeSystem(Args&&... args)
    {
        static_assert(is_convertible<T, System>::value, "Class must inherit from System");

        const auto system = make_shared<T>(this, forward<Args>(args)...);
        this->processSystems.emplace(system->get(), system);

        auto it = this->systemsByType.find(system->getType());
        if (it == this->systemsByType.end())
        {
            unordered_set<System *> set;
            it = this->systemsByType.emplace(system->getType(), set).first;
        }
        it->second.emplace(system);

        return system->get();
    }

    /**
     * Retrieves a weak reference of a system attached to the process.
     * @param system    The attached system.
     * @return  A weak reference to the provided system. Will link to nullptr if the provided system is not attached to
     *          the process.
     */
    weak_ptr<System> getWeakReference(System *system);

    /**
     * Detaches a system from the process, meaning it should be deleted (if there is no other shared reference active
     * at the moment).
     * @param system    The system to detach.
     * @return `true` if the provided system was attached. `false` otherwise.
     */
    bool detachSystem(System *system);

protected:

    /// The set of systems attached to this process, mapping the pointer to its corresponding shared pointer.
    unordered_map<System *, shared_ptr<System>> processSystems;

    /// A map of processes by type
    unordered_map<SystemTypes, unordered_set<System *>> systemsByType;


// ===========
// Helper functions
// ===========


public:

    /**
     * Helper function for retrieving the current player input handler.
     * @return The PlayerInput for the current graphic context
     */
    PlayerInput *getPlayerInput();

};


#endif //KRYNE_ENGINE_PROCESS_H
