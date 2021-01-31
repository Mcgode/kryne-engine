//
// Created by Max Godefroy on 20/11/2020.
//

#include "kryne-engine/3DObjects/Scene.h"


void Scene::add(Transform *transform)
{
    this->sceneEntities.emplace(transform->getEntity());

    for (const auto child : transform->getChildren())
        this->add(child);
}


void Scene::remove(Transform *transform)
{
    this->sceneEntities.erase(transform->getEntity());

    for (const auto child : transform->getChildren())
        this->remove(child);
}
