/**
 * @file
 * @author Max Godefroy
 * @date 16/12/2020.
 */

#ifndef KRYNE_ENGINE_ORBITCONTROLSCOMPONENT_H
#define KRYNE_ENGINE_ORBITCONTROLSCOMPONENT_H

#include <kryne-engine/Input/PlayerInput.h>
#include <kryne-engine/Core/LogicComponent.h>
#include "Camera.h"


/**
 * A camera control component that allows for orbit controls around a center position
 */
class OrbitControlsComponent : public LogicComponent {

public:

    /**
     * Initializes the camera control component.
     * @param entity The entity to attach this control component to
     */
    explicit OrbitControlsComponent(Entity *entity);

    /**
     * @copydoc Object3D::updateTransform()
     */
    void onUpdate() override;

    /**
     * @copydoc Object3D::setPosition()
     */
    void setPosition(const vec3 &pos);

    /**
     * Changes the value of #centerPosition and updates the camera position accordingly.
     * @param newCenter The new center position for the orbit
     */
    void setCenterPosition(const vec3 &newCenter) {
        OrbitControlsComponent::centerPosition = newCenter;
        OrbitControlsComponent::updatePosition();
    }


protected:

    /**
     * Updates OrbitCamera#position according to the orbit values
     */
    void updatePosition();

protected:

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


public:

    /// @copydoc Component::renderComponentDetails
    void renderComponentDetails() override;

};


#endif //KRYNE_ENGINE_ORBITCONTROLSCOMPONENT_H
