/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once


#include <memory>
#include <glm/mat4x4.hpp>

#include "ProjectionData.hpp"


using namespace glm;
using namespace std;


class ICamera {

public:

    virtual const mat4 &getProjectionMatrix() = 0;

    /**
     * Retrieves the inverse of the projection matrix of the camera.
     * @return A const reference to the projection matrix inverse
     */
    virtual const mat4 &getInverseProjectionMatrix() = 0;

    /**
     * Retrieves the view matrix of the camera.
     * @return A const reference to the view matrix.
     */
    [[nodiscard]] virtual const mat4 &getViewMatrix() const = 0;

    /**
     * Overrides the projection data for this camera
     * @param newProjectionData
     */
    virtual void setProjectionData(unique_ptr<ProjectionData> newProjectionData) = 0;

};