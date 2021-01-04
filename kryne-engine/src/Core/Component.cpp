/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#include "kryne-engine/Core/Component.h"


void Component::onBegin()
{
    this->begun = true;
}


inline const Transform *Component::getTransform() const
{
    return this->associatedEntity->getTransform();
}


template<typename T, typename... Args>
inline T *Component::addComponent(Args &&...args)
{
    return this->associatedEntity->addComponent<T>(args);
}


template<class C>
inline C *Component::getComponent() const
{
    return this->associatedEntity->getComponent<C>();
}


template<class C>
vector<C *> Component::getComponents() const
{
    return this->associatedEntity->getComponents<C>();
}
