/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#include "kryne-engine/Core/Process.h"


Process::Process(GraphicContext *context) : context(context)
{
//    for (size_t i = 0; i < SystemTypes::COUNT; i++)
//        this->systemsByType.emplace_back();

    for (auto &vec : this->systemsByType)
        vec = vector<System *>();
}


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

        auto vec = this->systemsByType[system->getType()];
        vec.erase(find(vec.begin(), vec.end(), system));

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
    using std::chrono::system_clock;

    this->lastFrameTimeData = this->frameTimer.resetTimer();

    auto initializeFrameTime = system_clock::now();

    if (this->currentScene == nullptr && !this->scenes.empty())
        this->currentScene = this->scenes.begin()->get();

    const auto renderer = this->context->getRenderer();

    vector<UIRenderer *> activeUIRenderers;
    for (const auto &uiRenderer : this->uiRenderers)
    {
        if (uiRenderer->isEnabled())
            activeUIRenderers.push_back(uiRenderer.get());
    }

    for (const auto &systemPair : this->processSystems)
        systemPair.first->loopReset();

    system_clock::time_point objectsRunTime;
    if (this->currentScene != nullptr)
    {
        for (const auto entity : this->currentScene->getEntities())
            entity->ranPreRenderingProcessing = false;

        auto entities = renderer->parseScene(this->currentScene);
        for (const auto &systemPair : this->processSystems)
            systemPair.first->parseScene(this->currentScene, entities);

        this->runPriorityPreProcesses(entities);

        renderer->prepareFrame();

        objectsRunTime = system_clock::now();
        for (const auto entity : this->currentScene->getTopLevelEntities())
            this->processEntity(entity, renderer);
    }
    else
    {
        objectsRunTime = system_clock::now();
        cerr << "There is no scene for the process." << endl;
    }

    Dispatcher::instance().waitMain();

    auto startRenderingTime = system_clock::now();
    auto postRenderScriptingTime = startRenderingTime;
    auto postProcessingTime = startRenderingTime;

    if (this->currentScene != nullptr)
    {
        renderer->finishSceneRendering(this->currentScene);

        postRenderScriptingTime = system_clock::now();

        if (!this->systemsByType[PostRendering].empty())
        {
            for (const auto entity : this->currentScene->getTopLevelEntities())
                this->handlePostRender(entity);
            Dispatcher::instance().waitMain();
        }

        postProcessingTime = system_clock::now();

        renderer->handlePostProcessing();
    }

    auto uiTime = system_clock::now();

    this->getPlayerInput()->setExternallyCaptured(false, false);
    for (auto uiRenderer : activeUIRenderers)
        uiRenderer->render(this);

    auto delayedTime = system_clock::now();

    Dispatcher::instance().waitDelayed();

    auto endFrameTime = system_clock::now();

    this->context->endFrame();

    this->frameTimer.recordTime("Initialization scripting", objectsRunTime - initializeFrameTime);
    this->frameTimer.recordTime("Objects scripting", startRenderingTime - objectsRunTime + postProcessingTime - postRenderScriptingTime);
    this->frameTimer.recordTime("Rendering", uiTime - postProcessingTime + postRenderScriptingTime - startRenderingTime);
    this->frameTimer.recordTime("UI", delayedTime - uiTime);
    this->frameTimer.recordTime("Delayed scripting", endFrameTime - delayedTime);
    this->frameTimer.recordTime("Events polling", system_clock::now() - endFrameTime);
}


struct ProcessCommon
{
    static inline void PriorityPreProcess(Entity *entity, const vector<System *> *systemsByType)
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

            {

#if KRYNE_ENGINE_SINGLE_THREADED != 1

                // Lock entity processing, to solve potential data racing issues.
                // Data racing can occur because the same entities can be called multiple times (like common parents),
                // on potentially different threads.
                scoped_lock<mutex> lock(entityToProcess->preRenderingProcessingMutex);

#endif

                if (entityToProcess->ranPreRenderingProcessing)
                    continue;

                for (const auto& system : systemsByType[LoopStart])
                    system->runSystem(entity);

                for (const auto& system : systemsByType[GameLogic])
                    system->runSystem(entity);

                for (const auto& system : systemsByType[PostLogic])
                    system->runSystem(entity);

                entityToProcess->ranPreRenderingProcessing = true;
            }
        }
    }


    static inline vector<RenderMesh *> PreRenderingFunction(Entity *entity, LoopRenderer *renderer,
                                                            const vector<System *> *systemsByType)
    {
        // No data race can happen in this state, since any entity is only called once by all parallel threads.
        // As a consequence, no fancy lock operation is needed.
        if (!entity->ranPreRenderingProcessing)
        {
            for (const auto& system : systemsByType[LoopStart])
                system->runSystem(entity);

            for (const auto& system : systemsByType[GameLogic])
                system->runSystem(entity);

            for (const auto& system : systemsByType[PostLogic])
                system->runSystem(entity);

            entity->ranPreRenderingProcessing = true;
        }

        for (const auto& system : systemsByType[PreRendering])
            system->runSystem(entity);

        auto renderMeshes = entity->getComponents<RenderMesh>(true);
        for (auto renderMesh : renderMeshes)
            renderer->computeFrustumCulling(renderMesh);

        return renderMeshes;
    }


    static inline void PostRenderingFunction(Entity *entity, const Process *process)
    {
        Entity *currentEntity = entity;

        while (currentEntity != nullptr)
        {
            for (const auto &system : process->systemsByType[PostRendering])
                system->runSystem(currentEntity);

            const auto children = currentEntity->getTransform()->getChildren();

            if (children.empty())
                currentEntity = nullptr;
            else
            {
                currentEntity = children[0]->getEntity();
                for (auto i = 1; i < children.size(); i++)
                    process->handlePostRender(children[i]->getEntity());
            }
        }
    }

};


void Process::runPriorityPreProcesses(const unordered_set<Entity *> &entities) const
{
    for (const auto entity : entities)
    {
        if (!entity->isEnabled())
            continue;

#if KRYNE_ENGINE_SINGLE_THREADED != 1

        Dispatcher::instance().parallel()->enqueue([this, entity]()
        {
            ProcessCommon::PriorityPreProcess(entity, this->systemsByType);
        });

#else

        ProcessCommon::PriorityPreProcess(entity, this->systemsByType);

#endif

    }

    Dispatcher::instance().waitMain();
}


void Process::processEntity(Entity *entity, LoopRenderer *renderer) const
{
    if (!entity->isEnabled())
        return;

#if KRYNE_ENGINE_SINGLE_THREADED != 1

    Dispatcher::instance().parallel()->enqueue([this, entity, renderer]()
    {
        Entity *currentEntity = entity;

        while (currentEntity != nullptr)
        {
            auto renderMeshes = ProcessCommon::PreRenderingFunction(currentEntity, renderer, this->systemsByType);

            Dispatcher::instance().main()->enqueue([renderer, renderMeshes]() {
                for (auto renderMesh : renderMeshes)
                    renderer->handleMesh(renderMesh);
            });

            const auto children = currentEntity->getTransform()->getChildren();

            if (children.empty())
                currentEntity = nullptr;
            else
            {
                currentEntity = children[0]->getEntity();
                for (auto i = 1; i < children.size(); i++)
                    this->processEntity(children[i]->getEntity(), renderer);
            }
        }

    });

#else

    auto meshes = ProcessCommon::PreRenderingFunction(entity, renderer, this->systemsByType);
    for (const auto mesh : meshes)
        renderer->handleMesh(mesh);

    for (const auto child: entity->getTransform()->getChildren())
        this->processEntity(child->getEntity(), renderer);

#endif
}


void Process::handlePostRender(Entity *entity) const
{
#if KRYNE_ENGINE_SINGLE_THREADED != 1

    Dispatcher::instance().parallel()->enqueue([entity, this]()
    {
        ProcessCommon::PostRenderingFunction(entity, this);
    });

#else

    ProcessCommon::PostRenderingFunction(entity, this);

#endif
}


PlayerInput *Process::getPlayerInput()
{
    return this->context->getPlayerInput();
}
