//
// Created by Max Godefroy on 22/11/2020.
//

#ifndef INC_3D_DEMOS_CAMERA_H
#define INC_3D_DEMOS_CAMERA_H

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

    void update(bool force) override {
        bool willUpdate = (force || this->matrixWorldNeedsUpdate) && this->visible;
        Object3D::update(force);
        if (willUpdate) {
            viewMatrix = inverse(matrixWorld);
        }
    }

    void setProjectionData(unique_ptr<ProjectionData> newProjectionData) {
        Camera::projectionData = std::move(newProjectionData);
    }

private:

    /// The projection data for this camera
    unique_ptr<ProjectionData> projectionData;

    mat4 viewMatrix {};

};


#endif //INC_3D_DEMOS_CAMERA_H
