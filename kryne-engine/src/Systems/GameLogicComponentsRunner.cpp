/**
 * @file
 * @author Max Godefroy
 * @date 23/02/2021.
 */

#include "kryne-engine/Systems/GameLogicComponentsRunner.h"

void GameLogicComponentsRunner::runSystem(Entity *entity)
{
    if (!entity->isEnabled())
        return;

    for (const auto logicComponent : entity->getComponents<LogicComponent>())
    {
        if (!logicComponent->hasBegun())
            logicComponent->onBegin();
        logicComponent->onUpdate();
    }
}

void GameLogicComponentsRunner::loopReset()
{

}
