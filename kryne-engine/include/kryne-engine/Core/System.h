/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#ifndef KRYNE_ENGINE_SYSTEM_H
#define KRYNE_ENGINE_SYSTEM_H


// Circular dependencies forward declaration
class Process;
class Scene;


/**
 * A system that will to be run at every frame by the game process
 */
class System {

public:

    /**
     * Initializes the system
     * @param process The process this system is attached to
     */
    explicit System(Process *process): attachedProcess(process) {}

    /**
     * Returns the process this system is attached to
     */
    [[nodiscard]] Process *getAttachedProcess() const { return attachedProcess; }

    /**
     * Runs the provided system. Called once every loop.
     *
     * @param scene                     The scene on which to run this process.
     *
     * @param allowConcurrentChunks     If `true`, the process expects systems to try and divide its current tasks into
     *                                  multiple chunks of code that can be run concurrently.
     *                                  You can ignore this flag if it is not relevant.
     *
     * @warning Expect systems to be run concurrently.
     */
    virtual void runSystem(Scene *scene, bool allowConcurrentChunks) = 0;

private:

    /// The process this system is attached to
    Process *attachedProcess;

};


// Circular dependencies includes
#include "Process.h"
#include <kryne-engine/3DObjects/Scene.h>


#endif //KRYNE_ENGINE_SYSTEM_H
