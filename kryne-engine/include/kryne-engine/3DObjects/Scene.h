/**
 * @file
 * @author Max Godefroy
 * @date 20/11/2020
 */

#ifndef INC_KRYNE_ENGINE_SCENE_H
#define INC_KRYNE_ENGINE_SCENE_H


#include "Transform.h"

class Scene : public Transform {

public:
    Scene(Entity *entity): Transform(entity) {}

};


#endif //INC_KRYNE_ENGINE_SCENE_H
