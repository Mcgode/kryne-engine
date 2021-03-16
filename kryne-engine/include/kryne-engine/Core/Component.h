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

    [[nodiscard]] Entity *getEntity() const { return this->associatedEntity; }

    /**
     * @brief A function triggered every time the entity's transform is updated.
     */
    virtual void transformDidUpdate() = 0;

    virtual ~Component() = default;

protected:

    /// The entity this component is attached to.
    Entity *associatedEntity;


// -----
// Name
// -----

public:

    [[nodiscard]] const string &getComponentName() const { return componentName; }

protected:

    string componentName = "Component";


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
     * @brief Enables/disables the component.
     *
     * @details
     * When disabled, a component won't be updated or rendered.
     *
     * @param newValue Set to `true` to enable, `false` to disable.
     */
    void setEnabled(bool newValue) { this->enabled = newValue; }

protected:

    /// Enabled status of the component.
    bool enabled = true;


// -----
// Entity helper functions
// -----

protected:

    /// @copydoc Entity::getTransform()
    [[nodiscard]] Transform *getTransform() const;

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
