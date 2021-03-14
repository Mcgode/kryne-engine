/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#ifndef KRYNE_ENGINE_LOGICCOMPONENT_H
#define KRYNE_ENGINE_LOGICCOMPONENT_H


#include "Component.h"


/**
 * A component to use for game logic.
 */
class LogicComponent: public Component {


public:

    /**
     * Initializes the logic component
     * @param entity    The entity this component will be attached to.
     *
     * @note A component should never be instanciated in a vaccum. Entity::addComponent() should be used instead.
     */
    explicit LogicComponent(Entity *entity): Component(entity) {}

    void transformDidUpdate() override {}


// -----
// Component lifecycle
// -----


public:

    /**
     * A function for just-in-time initializations.
     * This function is called just before its first #onUpdate() call;
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
    virtual void onAttach() {};

    /**
     * Update function called once per frame. Use it to run your component logic.
     */
    virtual void onUpdate() {};

    /**
     * Function called when the component is detached from its entity.
     * Won't be called if the component is simply destroyed along its attached entity.
     * @warning #onBegin() might not have been called yet.
     */
    virtual void onDetach() {};

private:

    bool begun = false;

};


#endif //KRYNE_ENGINE_LOGICCOMPONENT_H
