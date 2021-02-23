/**
 * @file
 * @author Max Godefroy
 * @date 23/02/2021.
 */

#pragma once


#include <kryne-engine/Core/System.h>
#include <kryne-engine/Core/LogicComponent.h>


class GameLogicComponentsRunner: public System {

public:

    explicit GameLogicComponentsRunner(Process *process): System(process, GameLogic) {}

    void runSystem(Entity *entity) override;

    void loopReset() override;

};


