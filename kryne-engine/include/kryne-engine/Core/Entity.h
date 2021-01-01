/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#ifndef KRYNE_ENGINE_ENTITY_H
#define KRYNE_ENGINE_ENTITY_H


class Process;


#include <atomic>
#include "Process.h"


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
     *
     * @return
     */
    inline Process *getProcess() const { return process; }

protected:

    /// The unique id of the object.
    uint32_t id;

private:

    /// The process this entity is attached to.
    Process *process;

};


#endif //KRYNE_ENGINE_ENTITY_H
