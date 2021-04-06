/**
 * @file
 * @author Max Godefroy
 * @date 05/04/2021.
 */

#pragma once


#include "Light.hpp"


/**
 * @brief An ambient light which color depends on the direction.
 */
class HemisphereLight : public Light {

public:

    /**
     * @brief Initializes an hemisphere light.
     *
     * @param process       The process this light is attached to.
     * @param skyColor      The up direction color.
     * @param groundColor   The down direction color.
     * @param intensity     The light intensity.
     * @param direction     The direction of the light.
     */
    explicit HemisphereLight(Process *process,
                             const vec3 &skyColor = vec3(1),
                             const vec3 &groundColor = vec3(0.2),
                             float intensity = 1.f,
                             const vec3 &direction = vec3(0, 0, 1));

    // Override
    void transformDidUpdate() override;

    /// @brief Retrieves the current sky color.
    [[nodiscard]] const vec3 &getSkyColor() const { return skyColor; }

    /// @brief Changes the ground color.
    void setSkyColor(const vec3 &newColor) { HemisphereLight::skyColor = newColor; }

    /// @brief Retrieves the current ground color.
    [[nodiscard]] const vec3 &getGroundColor() const { return groundColor; }

    /// @brief Changes the ground color.
    void setGroundColor(const vec3 &newColor) { HemisphereLight::groundColor = newColor; }

    /// @brief Retrieves the current light intensity
    [[nodiscard]] float getIntensity() const { return intensity; }

    /// @brief Updates the current light intensity
    void setIntensity(float value) { HemisphereLight::intensity = value; }

    /// @brief Retrieves the current direction of the light.
    [[nodiscard]] const vec3 &getDirection() const { return direction; }

    /// @brief Changes the current direction, and updates the world direction.
    void setDirection(const vec3 &newDirection)
    {
        this->direction = normalize(newDirection);
        this->worldDirection = this->getTransform()->getNormalMatrix() * this->direction;
    }

    /// @brief Retrieves the current world direction of the light
    [[nodiscard]] const vec3 &getWorldDirection() const { return worldDirection; }

    //Override
    void renderEntityDetails() override;

protected:

    /// The 'up' direction color
    vec3 skyColor;

    /// The 'down' direction color.
    vec3 groundColor;

    /// The intensity of the light
    float intensity;

    /// The direction of the light.
    vec3 direction;

    /// The world direction of the light. Automatically updated along the transform.
    vec3 worldDirection {};

};


