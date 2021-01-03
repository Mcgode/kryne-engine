/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#ifndef KRYNE_ENGINE_COMPONENT_H
#define KRYNE_ENGINE_COMPONENT_H


// Circular dependencies forward declaration
class Entity;


class Component {

public:

    explicit Component(Entity *entity): associatedEntity(entity) {}

protected:

    Entity *associatedEntity;

};


// Circular dependencies includes
#include "Entity.h"


#endif //KRYNE_ENGINE_COMPONENT_H
