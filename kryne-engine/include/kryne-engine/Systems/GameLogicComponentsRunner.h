/**
 * @file
 * @author Max Godefroy
 * @date 23/02/2021.
 */

#pragma once


#include <kryne-engine/Core/System.h>
#include <kryne-engine/Core/LogicComponent.h>


/**
 * @brief A system for running #LogicComponent instances in the scene
 */
class GameLogicComponentsRunner: public System {

public:

    // Override
    explicit GameLogicComponentsRunner(Process *process): System(process, GameLogic) {}

    // Override
    void runSystem(Entity *entity) override;

    // Override
    void loopReset() override;

    // Override
    void parseScene(Scene *scene, unordered_set<Entity *> &priorityEntities) override {}

};


