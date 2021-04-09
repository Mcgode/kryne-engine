/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#include <functional>
#include "WorkComponent.hpp"


WorkComponent::WorkComponent(Entity *entity) : LogicComponent(entity) {}


void WorkComponent::onUpdate()
{
    LogicComponent::onUpdate();

#ifndef NDEBUG
    const size_t N = 1000;
#else
    const size_t N = 10000;
#endif

    uint64_t h = 0;
    for (auto i = 0; i < N; i++)
        h = std::hash<uint64_t>{}(h);
}
