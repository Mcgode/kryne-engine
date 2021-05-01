/**
 * @file
 * @author Max Godefroy
 * @date 18/04/2021.
 */

#pragma once

#include <kryne-engine/Core/Entity.h>
#include <kryne-engine/Rendering/RenderMesh.h>
#include <kryne-engine/Camera/PerspectiveProjectionData.hpp>
#include <kryne-engine/Material/ShaderMaterial.hpp>


class PerspectiveHelper : public Entity {

public:

    PerspectiveHelper(Process *process, const weak_ptr<Entity> &camera);


protected:

    weak_ptr<Entity> camera;

};


