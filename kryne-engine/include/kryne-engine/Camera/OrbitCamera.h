/**
 * @file
 * @author Max Godefroy
 * @date 16/12/2020.
 */

#ifndef KRYNE_ENGINE_ORBITCAMERA_H
#define KRYNE_ENGINE_ORBITCAMERA_H

#include <kryne-engine/Input/PlayerInput.h>
#include "Camera.h"


/**
 * A camera that allows for orbit controls around a center position
 */
class OrbitCamera : public Camera {

public:

    /**
     * Initializes the camera.
     * @param projectionData    The projection data for this camera.
     * @param playerInput       The input controller associated with this camera.
     */
    OrbitCamera(unique_ptr<ProjectionData> projectionData, weak_ptr<PlayerInput> playerInput);

    /**
     * @copydoc Object3D::updateTransform()
     */
    void updateTransform(bool force) override;

    /**
     * @copydoc Object3D::setPosition()
     */
    void setPosition(const vec3 &pos) override;

    /**
     * Changes the value of #centerPosition and updates the camera position accordingly.
     * @param newCenter The new center position for the orbit
     */
    void setCenterPosition(const vec3 &newCenter) {
        OrbitCamera::centerPosition = newCenter;
        OrbitCamera::updatePosition();
    }


protected:

    /**
     * Updates OrbitCamera#position according to the orbit values
     */
    void updatePosition();

protected:

    /// The player input for handling the orbit controls.
    weak_ptr<PlayerInput> playerInput;

    /// The orbit center position
    vec3 centerPosition {};

    /// The spherical angle coordinates of the camera
    vec2 angle {};

    /// The distance of the camera from the orbit center
    float distance {};

    /// The speed of the swipe gesture, in radians per pixel
    float radiansPerMousePixel = 2.0f * glm::pi<float>() / 1280.0f;

    /// The max value for `abs(OrbitCamera::angle.y)`
    float maxPhi = pi<float>() * 0.48;

    /// The zoom speed for the scrolling input
    float scrollZoomSpeed = 0.2f;

};


#endif //KRYNE_ENGINE_ORBITCAMERA_H
