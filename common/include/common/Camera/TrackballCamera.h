//
// Created by max on 21/05/19.
//

#ifndef INC_3D_DEMOS_TRACKBALLCAMERA_H
#define INC_3D_DEMOS_TRACKBALLCAMERA_H

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include "Camera.h"


/**
 * A trackball camera, a camera allowing the user to rotate the camera around a given point, zoom in and out of this
 * point and move this point in the plane orthogonal to the current camera direction
 */
class TrackballCamera: public Camera {

public:
    /**
     * A trackball camera, a camera allowing the user to rotate the camera around a given point, zoom in and out of this
     * point and move this point in the plane orthogonal to the current camera direction
     *
     * @param centerPoint   The initial center point of the camera
     */
    explicit TrackballCamera(glm::vec3 centerPoint, float zoomDistance = 3.0f);

    glm::mat4 getViewMatrix() override;

    glm::vec3 getCurrentPosition() override;

    void onScrollInput(Window *window, float value) override;

    void onKeyInput(Window *window, GLint key) override {};

    void onMouseMovementInput(Window *window, double xInput, double yInput) override;

private:
    /// The current center position of the camera
    glm::vec3 centerPosition{};

    /// The current zooming distance of the camera
    float zoomDistance;

    /// The inclination angle of the trackball
    float phi;

    /// The rotation angle of the trackball
    float theta;

    bool wasMoving = false; ///< Keeps track whether the trackball was already moved during last input
    double lastMouseXPosition = 0; ///< The x position of the mouse cursor during the last update
    double lastMouseYPosition = 0; ///< The y position of the mouse cursor during the last update

    /**
     * Retrieves the current rotation of the camera
     * @return A 4x4 transform matrix with the rotation of the camera view
     */
    glm::mat4 getRotation();

};


#endif //INC_3D_DEMOS_TRACKBALLCAMERA_H
