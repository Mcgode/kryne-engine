/**
 * @file
 * @author Max Godefroy
 * @date 22/11/2020
 */

#ifndef INC_KRYNE_ENGINE_CAMERA_H
#define INC_KRYNE_ENGINE_CAMERA_H

#include <memory>
#include <kryne-engine/3DObjects/Object3D.h>
#include "ProjectionData.hpp"

class Camera : public Object3D {

public:

    explicit Camera(unique_ptr<ProjectionData> projectionData) : projectionData(std::move(projectionData)) {}

    const mat4 &getProjectionMatrix() {
        return projectionData->getProjectionMatrix();
    }

    const mat4 &getInverseProjectionMatrix() {
        return projectionData->getInverseProjectionMatrix();
    }

    [[nodiscard]] const mat4 &getViewMatrix() const {
        return viewMatrix;
    }

    void updateTransform(bool force) override {
        bool willUpdate = (force || this->matrixWorldNeedsUpdate) && this->visible;
        Object3D::updateTransform(force);
        if (willUpdate) {
            viewMatrix = inverse(matrixWorld);
        }
    }

    virtual void lookAt(const glm::vec3 &target = glm::vec3(0), const glm::vec3 &up = glm::vec3(0, 1, 0)) {
        this->applyLookAt(target, this->getWorldPosition(), up);
    }

    void setProjectionData(unique_ptr<ProjectionData> newProjectionData) {
        Camera::projectionData = std::move(newProjectionData);
    }

private:

    /// The projection data for this camera
    unique_ptr<ProjectionData> projectionData;

    mat4 viewMatrix {};

};


#endif //INC_KRYNE_ENGINE_CAMERA_H
