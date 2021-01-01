/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#ifndef KRYNE_ENGINE_COMPONENT_H
#define KRYNE_ENGINE_COMPONENT_H


#include "Entity.h"


class Component {

public:

    explicit Component(Entity *entity): associatedEntity(entity) {}

protected:

    Entity *associatedEntity;

};


#endif //KRYNE_ENGINE_COMPONENT_H
