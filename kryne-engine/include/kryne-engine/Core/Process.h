/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#ifndef KRYNE_ENGINE_PROCESS_H
#define KRYNE_ENGINE_PROCESS_H

class Entity;


#include <memory>
#include <unordered_map>
#include "Entity.h"


using namespace std;


class Process {

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
    inline T *makeEntity(Args&&... args);

    /**
     * Retrieves a weak reference of an entity attached to the process.
     * @param entity    The attached entity.
     * @return  A weak reference to the provided entity. Will link to nullptr if the provided entity is not attached to
     *          the process.
     */
    weak_ptr<Entity *> getWeakReference(Entity *entity);

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

};


#endif //KRYNE_ENGINE_PROCESS_H
