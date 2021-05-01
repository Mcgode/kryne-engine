/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#ifndef KRYNE_ENGINE_SYSTEM_H
#define KRYNE_ENGINE_SYSTEM_H


#include <unordered_set>
#include <kryne-engine/enums/SystemTypes.h>


// Circular dependencies forward declaration
class Process;
class Entity;
class Scene;


/**
 * @brief A system that will to be run at every frame by the game process
 */
class System {

public:

    /**
     * @brief Initializes the system
     *
     * @param process   The process this system is attached to.
     * @param type      The type of this system.
     */
    explicit System(Process *process, SystemTypes type = PostRendering): attachedProcess(process), type(type) {}

    /**
     * @brief Returns the process this system is attached to
     */
    [[nodiscard]] Process *getAttachedProcess() const { return attachedProcess; }

    /**
     * @brief Runs the system.
     *
     * @details
     * Called once per entity every loop.
     *
     * @param entity    The entity to run this system for.
     */
    virtual void runSystem(Entity *entity) = 0;

    /**
     * @brief A function that is called once, at the start of every loop.
     */
    virtual void loopReset() = 0;

    /**
     * @brief Retrieves the current system type
     */
    [[nodiscard]] SystemTypes getType() const { return type; }

    /**
     * @brief A function for parsing
     *
     * @param scene
     * @param priorityEntities
     */
    virtual void parseScene(Scene *scene, std::unordered_set<Entity *> &priorityEntities) = 0;

protected:

    /// The type of this system.
    SystemTypes type;

private:

    /// The process this system is attached to
    Process *attachedProcess;

};


// Circular dependencies includes
#include "Process.h"
#include "Entity.h"
#include "kryne-engine/3DObjects/Scene.h"


#endif //KRYNE_ENGINE_SYSTEM_H
