//
// Created by Max Godefroy on 20/11/2020.
//

#include "kryne-engine/3DObjects/Scene.h"


void Scene::add(Transform *transform)
{
    if (transform->parent != nullptr && transform->parent->attachedScene != this)
        transform->removeFromParent();

    if (transform->attachedScene != nullptr)
        transform->attachedScene->remove(transform);

    queue<Transform *> transformQueue;
    transformQueue.push(transform);

    while (!transformQueue.empty())
    {
        const auto t = transformQueue.front();
        this->sceneEntities.emplace(t->getEntity());
        t->attachedScene = this;

        for (const auto child : t->children)
            transformQueue.push(child);

        transformQueue.pop();
    }
}


void Scene::remove(Transform *transform)
{
    queue<Transform *> transformQueue;
    transformQueue.push(transform);

    while (!transformQueue.empty())
    {
        const auto t = transformQueue.front();
        this->sceneEntities.erase(t->getEntity());
        t->attachedScene = nullptr;

        for (const auto child : t->children)
            transformQueue.push(child);

        transformQueue.pop();
    }
}


const unordered_set<Entity *> &Scene::getTopLevelEntities()
{
    this->topLevelEntities.clear();

    for (Entity *entity : this->sceneEntities)
    {
        if (entity->getTransform()->getParent() == nullptr)
            this->topLevelEntities.insert(entity);
    }

    return this->topLevelEntities;
}
