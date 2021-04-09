/**
 * @file
 * @author Max Godefroy
 * @date 28/03/2021.
 */

#pragma once


#include <kryne-engine/Core/LogicComponent.h>


class WorkComponent : public LogicComponent {

public:

    explicit WorkComponent(Entity *entity);

    void onUpdate() override;

};


