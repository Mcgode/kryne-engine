//
// Created by Max Godefroy on 22/11/2020.
//

#include "kryne-engine/Camera/Camera.h"

void Camera::setProjectionData(unique_ptr<ProjectionData> newProjectionData)
{
    this->projectionData = std::move(newProjectionData);
}


void Camera::transformDidUpdate()
{
    viewMatrix = inverse(this->transform.getWorldMatrix());
}
