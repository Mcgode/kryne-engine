/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once

#include <kryne-engine/Camera/ICamera.hpp>
#include <kryne-engine/Geometry/BufferGeometry.h>
#include <kryne-engine/Material/Material.h>

class IRenderMesh {

public:

    virtual void onBeforeRender(const ICamera *camera) = 0;

    [[nodiscard]] virtual const shared_ptr<BufferGeometry> &getGeometry() const = 0;

    [[nodiscard]] virtual const shared_ptr<Material> &getMaterial() const = 0;

};