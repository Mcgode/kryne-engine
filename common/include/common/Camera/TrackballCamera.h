//
// Created by max on 21/05/19.
//

#ifndef INC_3D_DEMOS_TRACKBALLCAMERA_H
#define INC_3D_DEMOS_TRACKBALLCAMERA_H

#include <glm/gtc/quaternion.hpp>
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

private:
    /// The current center position of the camera
    glm::vec3 centerPosition{};

    /// The current zooming distance of the camera
    float zoomDistance;

    /// The quaternion representing the current rotation of the camera
    glm::quat rotation{};

};


#endif //INC_3D_DEMOS_TRACKBALLCAMERA_H
