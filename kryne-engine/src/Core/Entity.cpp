/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#include "kryne-engine/Core/Entity.h"

Entity::Entity(Process *process):
    id(Entity::nextId()),
    process(process)
{}


uint32_t Entity::nextId()
{
    static atomic<uint32_t> counter {0};
    return ++counter;
}


template<typename T, typename... Args>
inline T *Entity::addComponent(Args &&... args)
{
    static_assert(is_convertible<T, Component>::value, "Class must inherit from Component");

    const auto component = make_shared<T>(this, forward<Args>(args)...);
    this->components.emplace(component);
    return component->get();
}


template<class C>
C *Entity::getComponent()
{
    for (const auto & it : this->components)
    {
        auto component = dynamic_cast<C *>(it.get());
        if (component)
            return component;
    }
    return nullptr;
}


template<class C>
vector<C *> Entity::getComponents()
{
    vector<C *> result {};

    for (const auto &it : this->components)
    {
        const auto component = dynamic_cast<C *>(it.get());
        if (component)
            result.push_back(component);
    }

    return result;
}
