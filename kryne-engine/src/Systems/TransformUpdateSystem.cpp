/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#include "kryne-engine/Systems/TransformUpdateSystem.h"

void TransformUpdateSystem::runSystem(Entity *entity)
{
    if (entity->isEnabled())
        entity->getTransform()->updateTransform(false, false);
}


