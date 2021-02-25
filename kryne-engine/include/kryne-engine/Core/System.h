/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#ifndef KRYNE_ENGINE_SYSTEM_H
#define KRYNE_ENGINE_SYSTEM_H


#include <kryne-engine/enums/SystemTypes.h>


// Circular dependencies forward declaration
class Process;
class Entity;


/**
 * A system that will to be run at every frame by the game process
 */
class System {

public:

    /**
     * Initializes the system
     * @param process   The process this system is attached to.
     * @param type      The type of this system.
     */
    explicit System(Process *process, SystemTypes type = PostRendering): attachedProcess(process), type(type) {}

    /**
     * Returns the process this system is attached to
     */
    [[nodiscard]] Process *getAttachedProcess() const { return attachedProcess; }

    /**
     * Runs the system. Called once per entity every loop.
     *
     * @param entity    The entity to run this system for.
     */
    virtual void runSystem(Entity *entity) = 0;

    /**
     * A function that is called once, at the start of every loop.
     */
    virtual void loopReset() = 0;

    [[nodiscard]] SystemTypes getType() const { return type; }

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


#endif //KRYNE_ENGINE_SYSTEM_H
