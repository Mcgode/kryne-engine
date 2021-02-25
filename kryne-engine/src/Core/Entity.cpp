/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#include "kryne-engine/Core/Entity.h"

Entity::Entity(Process *process):
    id(Entity::nextId()),
    process(process),
    transform(make_unique<Transform>(this))
{}


uint32_t Entity::nextId()
{
    static atomic<uint32_t> counter {0};
    return ++counter;
}
