/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#pragma once


#include <kryne-engine/Core/LogicComponent.h>


class WorkComponent : public LogicComponent {

public:

    WorkComponent(Entity *entity);

    void onUpdate() override;

protected:

    static uint64_t fibonacci(uint64_t n);

};


