/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once


#include <mutex>

#include <kryne-engine/Core/System.h>


class TransformUpdateSystem : public System {

public:

    explicit TransformUpdateSystem(Process *process): System(process, PreRendering) {}

    void runSystem(Entity *entity) override;

    void loopReset() override;

protected:

    void updateTransform(Transform *transform);

protected:

    unordered_set<Transform *> updatedTransforms {};

    unordered_map<Transform *, vector<Transform *>> toBeUpdated {};

    mutex parallelMutex {};

};


