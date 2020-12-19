/**
 * @file
 * @author Max Godefroy
 * @date 16/12/2020.
 */

#ifndef KRYNE_ENGINE_ORBITCAMERA_H
#define KRYNE_ENGINE_ORBITCAMERA_H

#include <kryne-engine/Input/PlayerInput.h>
#include "Camera.h"


class OrbitCamera : public Camera {

public:

    OrbitCamera(unique_ptr<ProjectionData> projectionData, weak_ptr<PlayerInput> playerInput);

    void update(bool force) override;

    void setPosition(const vec3 &pos) override;


protected:

    void updatePosition(const vec2 &newAngle, float dist);

protected:

    weak_ptr<PlayerInput> playerInput;

    vec3 centerPosition {};

    vec2 angle {};

    float distance {};

    float radiansPerMousePixel = glm::pi<float>() / 1280.0f;

    float maxPhi = pi<float>() * 0.48;

};


#endif //KRYNE_ENGINE_ORBITCAMERA_H
