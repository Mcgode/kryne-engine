/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#include "kryne-engine/Core/Process.h"


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


weak_ptr<System> Process::getWeakReference(System *system)
{
    const auto it = this->processSystems.find(system);

    weak_ptr<System> ptr;
    if (it != this->processSystems.end())
        ptr = it->second;

    return ptr;
}


bool Process::detachSystem(System *system)
{
    const auto it = this->processSystems.find(system);

    if (it != this->processSystems.end())
    {
        this->processSystems.erase(it);

        const auto it2 = this->systemsByType.find(system->getType());
        if ( it2 != this->systemsByType.end() )
            it2->second.erase(system);

        return true;
    }

    return false;
}


Scene *Process::makeScene()
{
    unique_ptr<Scene> scene = make_unique<Scene>();
    const auto p = scene.get();
    scenes.emplace(move(scene));
    return p;
}


void Process::runLoop()
{
    if (this->currentScene == nullptr && !this->scenes.empty())
        this->currentScene = this->scenes.begin()->get();

    if (this->currentScene != nullptr)
    {
        const auto renderer = this->context->getRenderer();

        renderer->prepareFrame();

        for (const auto entity : this->currentScene->getEntities()) 
        {
            auto it = this->systemsByType.find(PreRendering);
            if (it != this->systemsByType.end())
            {
                // TODO: multithread this
                for (const auto& systemPair : it->second)
                    systemPair->runSystem(entity);
            }

            it = this->systemsByType.find(GameLogic);
            if (it != this->systemsByType.end())
            {
                // TODO: multithread this
                for (const auto& systemPair : it->second)
                    systemPair->runSystem(entity);
            }

            it = this->systemsByType.find(PreRendering);
            if (it != this->systemsByType.end())
            {
                // TODO: multithread this
                for (const auto& systemPair : it->second)
                    systemPair->runSystem(entity);
            }

            // TODO: Multithread this
            for (auto renderMesh : entity->getComponents<RenderMesh>())
                renderer->handleMesh(renderMesh);

            it = this->systemsByType.find(PostRendering);
            if (it != this->systemsByType.end())
            {
                // TODO: multithread this
                for (const auto& systemPair : it->second)
                    systemPair->runSystem(entity);
            }
        }
    }
    else
        cerr << "There is no scene for the process." << endl;

    this->dispatcher->synchronize();

    this->dispatcher->launchAsyncProcessing();
    this->dispatcher->synchronize();

    this->context->endFrame();
}


PlayerInput *Process::getPlayerInput()
{
    return this->context->getPlayerInput();
}
