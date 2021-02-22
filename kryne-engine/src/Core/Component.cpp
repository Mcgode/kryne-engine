/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#include "kryne-engine/Core/Component.h"


Transform *Component::getTransform() const
{
    return this->associatedEntity->getTransform();
}


template<typename T, typename... Args>
T *Component::addComponent(Args &&...args)
{
    return this->associatedEntity->addComponent<T>(forward<Args>(args)...);
}


template<class C>
C *Component::getComponent() const
{
    return this->associatedEntity->getComponent<C>();
}


template<class C>
vector<C *> Component::getComponents() const
{
    return this->associatedEntity->getComponents<C>();
}
