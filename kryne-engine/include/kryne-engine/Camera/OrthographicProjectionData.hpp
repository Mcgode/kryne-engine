/**
 * @file
 * @author Max Godefroy
 * @date 18/04/2021.
 */

#pragma once


#include "ProjectionData.hpp"


class OrthographicProjectionData : public ProjectionData {

public:

    explicit OrthographicProjectionData(float depth = 0,
                                        float width = 0,
                                        float height = 0,
                                        const vec3 &offset = vec3(0)) :
                                            depth(depth),
                                            width(width),
                                            height(height),
                                            offset(offset)
    {
        OrthographicProjectionData::updateProjectionMatrix();
    }

    /// @brief Retrieves the projection depth
    [[nodiscard]] float getDepth() const { return depth; }

    /// @brief Updates the projection depth
    void setDepth(float value)
    {
        OrthographicProjectionData::depth = value;
        this->updateProjectionMatrix();
    }

    /// @brief Retrieves the projection width
    [[nodiscard]] float getWidth() const { return width; }

    /// @brief Updates the projection width
    void setWidth(float value)
    {
        OrthographicProjectionData::width = value;
        this->updateProjectionMatrix();
    }

    /// @brief Retrieves the projection height
    [[nodiscard]] float getHeight() const { return height; }

    /// @brief Updates the projection height
    void setHeight(float value)
    {
        OrthographicProjectionData::height = value;
        this->updateProjectionMatrix();
    }

    /// @brief Retrieves the projection offset
    [[nodiscard]] const vec3 &getOffset() const { return offset; }

    /// @brief Updates the projection offset
    void setOffset(const vec3 &value)
    {
        OrthographicProjectionData::offset = value;
        this->updateProjectionMatrix();
    }

    /**
     * @brief Updates the frustum values.
     *
     * @param min   The frustum box min vector.
     * @param max   The frustum box max vector.
     */
    void setFrustum(vec3 min, vec3 max)
    {
        vec3 diff = max - min;

        this->width = diff.x;
        this->height = diff.y;
        this->depth = diff.z;

        this->offset = vec3(min.x + diff.x / 2, min.y + diff.y / 2, min.z);

        this->updateProjectionMatrix();
    }

protected:

    // Override
    void updateProjectionMatrix() override
    {
        float right  = offset.x + width / 2,
              left   = offset.x - width / 2,
              top    = offset.y + height / 2,
              bottom = offset.y - height / 2,
              far    = offset.z + depth,
              near   = offset.z;

        this->projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
        this->inverseProjectionMatrix = glm::inverse(this->projectionMatrix);
    }

protected:

    /// The depth of the frustum. Used to compute far/near components.
    float depth = 0;

    /// The width of the frustum. Used to compute right/left components.
    float width = 0;

    /// The height of the frustum. Used to compute top/bottom components.
    float height = 0;

    /// The offset position of the frustum. This position corresponds to the center of the near plane.
    vec3 offset = vec3(0);

};


