//
// Created by Max Godefroy on 22/11/2020.
//

#ifndef INC_3D_DEMOS_PROJECTIONDATA_HPP
#define INC_3D_DEMOS_PROJECTIONDATA_HPP

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;

/**
 * An abstract method for handling camera projection data and calculate the associated matrices
 */
class ProjectionData {

public:

    /**
     * Returns the projection matrix calculated from the projection data
     */
    [[nodiscard]] const mat4 &getProjectionMatrix() const {
        return projectionMatrix;
    }


    /**
     * Returns the inverse projection matrix calculated from the projection data
     */
    [[nodiscard]] const mat4 &getInverseProjectionMatrix() const {
        return inverseProjectionMatrix;
    }

protected:

    /**
     * Abstract method that calculates the new values for the matrices.
     */
    virtual void updateProjectionMatrix() = 0;

    /// The projection matrix calculated from the data
    mat4 projectionMatrix {};

    /// The inverse projection matrix calculated from the data
    mat4 inverseProjectionMatrix{};

};

#endif //INC_3D_DEMOS_PROJECTIONDATA_HPP
