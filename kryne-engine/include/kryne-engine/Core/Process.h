/**
 * @file
 * @author Max Godefroy
 * @date 22/12/2020.
 */

#ifndef KRYNE_ENGINE_PROCESS_H
#define KRYNE_ENGINE_PROCESS_H

class Entity;


#include <memory>
#include "Entity.h"


using namespace std;


class Process {

public:

    template<typename T, typename... Args>
    inline unique_ptr<T> makeEntity(Args&&... args)
    {
        static_assert(is_constructible<T, Entity>(), "Class must inherit from Entity");
        return make_unique<T>(this, forward<Args>(args)...);
    }

};


#endif //KRYNE_ENGINE_PROCESS_H
