/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#include "kryne-engine/Core/Process.h"

template<typename T, typename... Args>
inline T *Process::makeEntity(Args &&... args)
{
    static_assert(is_convertible<T, Entity>::value, "Class must inherit from Entity");

    const auto entity = make_shared<T>(this, forward<Args>(args)...);
    this->processEntities.emplace(pair(entity->get(), entity));
    return entity->get();
}


weak_ptr<Entity> Process::getWeakReference(Entity *entity)
{
    const auto it = this->processEntities.find(entity);

    weak_ptr<Entity> ptr;
    if (it != this->processEntities.end())
        ptr = it->second;

    return ptr;
}


bool Process::detachEntity(Entity *entity)
{
    const auto it = this->processEntities.find(entity);

    if (it != this->processEntities.end())
    {
        this->processEntities.erase(it);
        return true;
    }

    return false;
}
