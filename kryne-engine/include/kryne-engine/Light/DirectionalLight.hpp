/**
 * @file
 * @author Max Godefroy
 * @date 06/04/2021.
 */

#pragma once


#include "Light.hpp"


/**
 * @brief A direct light shining in a specific direction, with no distance attenuation.
 */
class DirectionalLight : public Light {

public:

    /**
     * @brief Initializes an hemisphere light.
     *
     * @param process       The process this light is attached to.
     * @param color         The light color.
     * @param intensity     The light intensity.
     * @param direction     The direction of the light.
     */
    explicit DirectionalLight(Process *process,
                              const vec3 &color = vec3(1),
                              float intensity = 1.f,
                              const vec3 &direction = vec3(0, 1, 0));

    // Override
    void transformDidUpdate() override;

    /// @brief Retrieves the current light color.
    [[nodiscard]] const vec3 &getColor() const { return color; }

    /// @brief Changes the current light color.
    void setColor(const vec3 &value) { DirectionalLight::color = value; }

    /// @brief Retrieves the current light intensity.
    [[nodiscard]] float getIntensity() const { return intensity; }

    /// @brief Changes the current light intensity.
    void setIntensity(float value) { DirectionalLight::intensity = value; }

    /// @brief Retrieves the current light local direction.
    [[nodiscard]] const vec3 &getDirection() const { return direction; }

    /// @brief Changes the current light local direction, and updates the world direction accordingly.
    void setDirection(const vec3 &value)
    {
        this->direction = normalize(value);
        this->worldDirection = this->getTransform()->getNormalMatrix() * this->direction;
    }

    /// @brief Retrieves the current light direction in the world space.
    [[nodiscard]] const vec3 &getWorldDirection() const { return worldDirection; }

    // Override
    void renderEntityDetails() override;

protected:

    /// The light color.
    vec3 color;

    /// The light intensity.
    float intensity;

    /// The local-space direction of the light.
    vec3 direction;

    /// The world-space direction of the light.
    vec3 worldDirection;

};


