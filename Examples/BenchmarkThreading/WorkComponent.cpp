/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#include "WorkComponent.hpp"


WorkComponent::WorkComponent(Entity *entity) : LogicComponent(entity) {}


void WorkComponent::onUpdate()
{
    LogicComponent::onUpdate();

    fibonacci(20);
}

uint64_t WorkComponent::fibonacci(uint64_t n)
{
    if (n <= 0)
        return 0;
    else if (n == 1)
        return 1;
    else
        return  fibonacci(n - 2) + fibonacci(n - 1);
}
