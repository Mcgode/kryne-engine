/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#include "kryne-engine/Core/Process.h"

template<typename T, typename... Args>
inline T *Process::makeEntity(Args &&... args)
{
    static_assert(is_convertible<T, Entity>::value, "Class must inherit from Entity");

    const auto entity = make_shared<T>(this, forward<Args>(args)...);
    this->processEntities.emplace(pair(entity->get(), entity));
    return entity->get();
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


template<typename T, typename... Args>
T *Process::makeSystem(Args&&... args)
{
    static_assert(is_convertible<T, System>::value, "Class must inherit from System");

    const auto system = make_shared<T>(this, forward<Args>(args)...);
    this->processSystems.emplace(system->get(), system);

    auto it = this->systemsByType.find(system->getType());
    if (it == this->systemsByType.end())
    {
        unordered_set<System *> set;
        it = this->systemsByType.emplace(system->getType(), set).first;
    }
    it->second.emplace(system);

    return system->get();
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
        auto it = this->systemsByType.find(PreRendering);
        if (it != this->systemsByType.end())
        {
            // TODO: multithread this
            for (const auto& systemPair : it->second)
                systemPair->runSystem(this->currentScene, false);
        }

        it = this->systemsByType.find(GameLogic);
        if (it != this->systemsByType.end())
        {
            // TODO: multithread this
            for (const auto& systemPair : it->second)
                systemPair->runSystem(this->currentScene, false);
        }

        it = this->systemsByType.find(PreRendering);
        if (it != this->systemsByType.end())
        {
            // TODO: multithread this
            for (const auto& systemPair : it->second)
                systemPair->runSystem(this->currentScene, false);
        }

        it = this->systemsByType.find(PostRendering);
        if (it != this->systemsByType.end())
        {
            // TODO: multithread this
            for (const auto& systemPair : it->second)
                systemPair->runSystem(this->currentScene, false);
        }
    }
    else
        cerr << "There is no scene for the process." << endl;

    // TODO : see if anything else needs updating
}


inline PlayerInput *Process::getPlayerInput()
{
    return this->context->getPlayerInput();
}
