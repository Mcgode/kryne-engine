/**
 * @file
 * @author Max Godefroy
 * @date 05/04/2021.
 */

#pragma once


#include <kryne-engine/Core/Entity.h>


/**
 * @brief An entity that will provide light to the scene.
 */
class Light: public Entity {

protected:

    // Override
    explicit Light(Process *process): Entity(process) {}

};

