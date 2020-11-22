//
// Created by Max Godefroy on 22/11/2020.
//

#ifndef INC_3D_DEMOS_PERSPECTIVEPROJECTIONDATA_HPP
#define INC_3D_DEMOS_PERSPECTIVEPROJECTIONDATA_HPP

#include "ProjectionData.hpp"


/**
 * The data for a perspective projection
 */
class PerspectiveProjectionData : public ProjectionData {

public:

    /**
     * Initialize the perspective projection.
     * @param aspectRatio   Aspect ratio for the projection.
     * @param fieldOfView   Vertical field of view for the projection.
     * @param nearPlane     Minimum z value, corresponding to the near plane.
     * @param farPlane      Maximum z value, corresponding to the far plane.
     */
    explicit PerspectiveProjectionData(float aspectRatio,
                                       float fieldOfView = glm::pi<float>() / 2.,
                                       float nearPlane = 0.01f,
                                       float farPlane = 100.0f) :
        aspectRatio(aspectRatio),
        fieldOfView(fieldOfView),
        nearPlane(nearPlane),
        farPlane(farPlane)
    {
        PerspectiveProjectionData::updateProjectionMatrix();
    };

    /**
     * Returns the current aspect ratio
     */
    [[nodiscard]] float getAspectRatio() const {
        return aspectRatio;
    }

    /**
     * Overwrites the aspect ratio and updates the matrices
     * @param newAspectRatio The new aspect ratio value
     */
    void setAspectRatio(float newAspectRatio) {
        PerspectiveProjectionData::aspectRatio = newAspectRatio;
        updateProjectionMatrix();
    }

    /**
     * Returns the current field of view value
     */
    [[nodiscard]] float getFieldOfView() const {
        return fieldOfView;
    }

    /**
     * Overwrites the field of view and updates the matrices
     * @param newFieldOfView The new field of view value
     */
    void setFieldOfView(float newFieldOfView) {
        PerspectiveProjectionData::fieldOfView = newFieldOfView;
        updateProjectionMatrix();
    }

    /**
     * Returns the current near plane value
     */
    [[nodiscard]] float getNearPlane() const {
        return nearPlane;
    }

    /**
     * Overwrites the near plane z value and updates the matrices
     * @param newNearPlane The new near plane value
     */
    void setNearPlane(float newNearPlane) {
        PerspectiveProjectionData::nearPlane = newNearPlane;
        updateProjectionMatrix();
    }

    /**
     * Returns the current far plane value
     */
    [[nodiscard]] float getFarPlane() const {
        return farPlane;
    }

    /**
     * Overwrites the far plane z value and updates the matrices
     * @param newFarPlane The new far plane value
     */
    void setFarPlane(float newFarPlane) {
        PerspectiveProjectionData::farPlane = newFarPlane;
        updateProjectionMatrix();
    }

private:

    void updateProjectionMatrix() override {
        this->projectionMatrix = perspective(this->fieldOfView, this->aspectRatio, this->nearPlane, this->farPlane);
        this->inverseProjectionMatrix = inverse(this->projectionMatrix);
    }

    float aspectRatio;

    float fieldOfView;

    float nearPlane;

    float farPlane;
};


#endif //INC_3D_DEMOS_PERSPECTIVEPROJECTIONDATA_HPP
