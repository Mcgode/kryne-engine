/**
 * @file
 * @author Max Godefroy
 * @date 22/11/2020
 */

#ifndef INC_KRYNE_ENGINE_CAMERA_H
#define INC_KRYNE_ENGINE_CAMERA_H


#include <memory>
#include <glm/mat4x4.hpp>

#include <kryne-engine/Core/Entity.h>
#include "ProjectionData.hpp"


class Camera : public Entity {

public:

    /**
     * Initializes the camera entity. Using Process::makeEntity() is recommended
     * @param process           The process to attach this camera entity to.
     * @param projectionData    The projection data for this camera
     */
    explicit Camera(Process *process, unique_ptr<ProjectionData> projectionData) :
        Entity(process),
        projectionData(std::move(projectionData))
    {
        this->name = "Camera";
    }

    /// @copydoc Entity::transformDidUpdate
    void transformDidUpdate() override;

public:

    /**
     * Retrieves the projection matrix of the camera.
     * @return A const reference to the projection matrix
     */
    inline const mat4 &getProjectionMatrix() { return projectionData->getProjectionMatrix(); }

    /**
     * Retrieves the inverse of the projection matrix of the camera.
     * @return A const reference to the projection matrix inverse
     */
    inline const mat4 &getInverseProjectionMatrix() { return projectionData->getInverseProjectionMatrix(); }

    /**
     * Retrieves the view matrix of the camera.
     * @return A const reference to the view matrix.
     */
    [[nodiscard]] inline const mat4 &getViewMatrix() const { return viewMatrix; }

    /**
     * Overrides the projection data for this camera
     * @param newProjectionData
     */
    void setProjectionData(unique_ptr<ProjectionData> newProjectionData);

    /**
     * @brief Retrieves the current projection data
     */
     [[nodiscard]] ProjectionData *getProjectionData() const { return this->projectionData.get(); }

protected:

    /// The projection data for this camera
    unique_ptr<ProjectionData> projectionData;

    /// The view matrix of this camera
    mat4 viewMatrix {};

};


#endif //INC_KRYNE_ENGINE_CAMERA_H
