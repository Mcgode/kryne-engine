/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once


#include <mutex>

#include <kryne-engine/Core/System.h>


/**
 * @brief A system for updating entity transforms
 */
class TransformUpdateSystem : public System {

public:

    // Override constructor
    explicit TransformUpdateSystem(Process *process): System(process, PostLogic) {}

    // Override
    void runSystem(Entity *entity) override;

    // Override
    void loopReset() override {}

    // Override
    void parseScene(Scene *scene, unordered_set<Entity *> &priorityEntities) override {}

};


