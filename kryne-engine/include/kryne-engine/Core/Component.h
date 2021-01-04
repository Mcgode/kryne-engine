/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#ifndef KRYNE_ENGINE_COMPONENT_H
#define KRYNE_ENGINE_COMPONENT_H


// Circular dependencies forward declaration
class Entity;
class Transform;


#include <vector>


using namespace std;


class Component {


// -----
// Base
// -----


public:

    /**
     * Initializes and attaches the component to a entity.
     * @param entity The entity to attach the component to.
     */
    explicit Component(Entity *entity): associatedEntity(entity) {}

protected:

    /// The entity this component is attached to.
    Entity *associatedEntity;


// -----
// Component lifecycle
// -----


public:

    /**
     * A function for just-in-time initializations.
     * This function is called just before its first #update() call;
     */
    virtual void onBegin();

    /**
     * @return `true` if onBegin has already been called
     */
    [[nodiscard]] inline bool hasBegun() const { return this->begun; }

    /**
     * Function called when the component is attached to an entity
     * @warning #onBegin() might not have been called yet
     */
    virtual void onAttach() = 0;

    /**
     * Update function called once per frame. Use it to run your component logic.
     */
    virtual void onUpdate() = 0;

    /**
     * Function called when the component is detached from its entity.
     * Won't be called if the component is simply destroyed along its attached entity.
     * @warning #onBegin() might not have been called yet.
     */
     virtual void onDetach() = 0;

private:

    bool begun = false;


// -----
// Entity helper functions
// -----

protected:

    /// @copydoc Entity::getTransform()
    [[nodiscard]] const Transform *getTransform() const;

    /// @copydoc Entity::addComponent()
    template<typename T, typename... Args>
    T *addComponent(Args&&... args);

    /// @copydoc Entity::getComponent()
    template<class C>
    C *getComponent() const;

    /// @copydoc Entity::getComponents()
    template<class C>
    vector<C *> getComponents() const;
};


// Circular dependencies includes
#include "Entity.h"


#endif //KRYNE_ENGINE_COMPONENT_H
