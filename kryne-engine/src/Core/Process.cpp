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

    const auto renderer = this->context->getRenderer();

    vector<UIRenderer *> activeUIRenderers;
    for (const auto &uiRenderer : this->uiRenderers)
    {
        if (uiRenderer->isEnabled())
            activeUIRenderers.push_back(uiRenderer.get());
    }

    if (this->currentScene != nullptr)
    {
        for (const auto entity : this->currentScene->getEntities())
            entity->ranPreRenderingProcessing = false;

        auto entities = renderer->parseScene(this->currentScene);
        this->runPriorityPreProcesses(entities);

        renderer->prepareFrame();

        for (const auto entity : this->currentScene->getTopLevelEntities())
            this->processEntity(entity, renderer);
    }
    else
        cerr << "There is no scene for the process." << endl;

    Dispatcher::instance().waitMain();

    if (this->currentScene != nullptr)
        renderer->renderToScreen();

    for (auto uiRenderer : activeUIRenderers)
        uiRenderer->render(this);

    Dispatcher::instance().waitDelayed();

    this->context->endFrame();
}


void Process::processEntity(Entity *entity, LoopRenderer *renderer) const
{
    if (!entity->isEnabled())
        return;

    Dispatcher::instance().parallel()->enqueue([this, entity, renderer]()
    {
        // No data race can happen in this state, since any entity is only called once by all parallel threads.
        // As a consequence, no fancy lock operation is needed.
        if (!entity->ranPreRenderingProcessing)
        {
            auto it = this->systemsByType.find(LoopStart);
            if (it != this->systemsByType.end())
            {
                for (const auto& systemPair : it->second)
                    systemPair->runSystem(entity);
            }

            it = this->systemsByType.find(GameLogic);
            if (it != this->systemsByType.end())
            {
                for (const auto& systemPair : it->second)
                    systemPair->runSystem(entity);
            }

            it = this->systemsByType.find(PreRendering);
            if (it != this->systemsByType.end())
            {
                for (const auto& systemPair : it->second)
                    systemPair->runSystem(entity);
            }

            entity->ranPreRenderingProcessing = true;
        }

        const auto renderMeshes = entity->getComponents<RenderMesh>();
        for (auto renderMesh : renderMeshes)
            renderer->computeFrustumCulling(renderMesh);

        Dispatcher::instance().main()->enqueue([this, entity, renderer, renderMeshes]()
        {
            for (auto renderMesh : renderMeshes)
                renderer->handleMesh(renderMesh);

            Dispatcher::instance().parallel()->enqueue([this, entity, renderer]()
            {
                auto it = this->systemsByType.find(PostRendering);
                if (it != this->systemsByType.end())
                {
                    for (const auto& systemPair : it->second)
                        systemPair->runSystem(entity);
                }

                for (const auto child: entity->getTransform()->getChildren())
                    this->processEntity(child->getEntity(), renderer);
            });
        });
    });
}


void Process::runPriorityPreProcesses(const vector<Entity *> &entities) const
{
    for (const auto entity : entities)
    {
        if (!entity->isEnabled())
            continue;

        Dispatcher::instance().parallel()->enqueue([this, entity]()
        {
            stack<Entity *> processStack;

            Transform *current = entity->getTransform();
            do
            {
                processStack.push(current->getEntity());
                current = current->getParent();
            } while (current != nullptr);

            while (!processStack.empty())
            {
                Entity *entityToProcess = processStack.top();
                processStack.pop();

                // Lock entity processing, to solve potential data racing issues.
                // Data racing can occur because the same entities can be called multiple times (like common parents),
                // on potentially different threads.
                {
                    unique_lock<mutex> lock(entityToProcess->preRenderingProcessingMutex);

                    if (entityToProcess->ranPreRenderingProcessing)
                        continue;

                    auto it = this->systemsByType.find(LoopStart);
                    if (it != this->systemsByType.end())
                    {
                        for (const auto& systemPair : it->second)
                            systemPair->runSystem(entityToProcess);
                    }

                    it = this->systemsByType.find(GameLogic);
                    if (it != this->systemsByType.end())
                    {
                        for (const auto& systemPair : it->second)
                            systemPair->runSystem(entityToProcess);
                    }

                    it = this->systemsByType.find(PreRendering);
                    if (it != this->systemsByType.end())
                    {
                        for (const auto& systemPair : it->second)
                            systemPair->runSystem(entityToProcess);
                    }

                    entityToProcess->ranPreRenderingProcessing = true;
                }
            }
        });
    }

    Dispatcher::instance().waitMain();
}


PlayerInput *Process::getPlayerInput()
{
    return this->context->getPlayerInput();
}
