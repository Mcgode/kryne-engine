/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#include "TransformUpdateSystem.h"

void TransformUpdateSystem::runSystem(Entity *entity)
{
    if (entity->isEnabled())
    {
        const auto transform = entity->getTransform();
        const auto parent = transform->getParent();

        bool canUpdate = true;

        lock_guard<mutex> guard(this->parallelMutex);

        if (parent != nullptr)
        {
            const auto it = this->updatedTransforms.find(parent);
            canUpdate = it != this->updatedTransforms.end();
        }

        if (canUpdate)
        {
            this->updateTransform(transform);
        }
        else
        {
            auto it = this->toBeUpdated.find(parent);
            if (it == this->toBeUpdated.end())
                it = this->toBeUpdated.emplace(parent, vector<Transform *>()).first;
            it->second.push_back(transform);
        }
    }
}


void TransformUpdateSystem::updateTransform(Transform *transform)
{
    transform->updateTransform(false, false);

    const auto it = this->toBeUpdated.find(transform);
    if (it != this->toBeUpdated.end())
    {
        for (const auto child : it->second)
            this->updateTransform(child);
    }
}



void TransformUpdateSystem::loopReset()
{
    this->updatedTransforms.clear();
    this->toBeUpdated.clear();
}


