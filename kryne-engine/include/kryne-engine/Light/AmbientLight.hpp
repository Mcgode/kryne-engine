/**
 * @file
 * @author Max Godefroy
 * @date 06/04/2021.
 */

#pragma once


#include "Light.hpp"


/**
 * @brief A light that provides an ambient, indirect diffuse lighting.
 */
class AmbientLight: public Light {

public:

    /**
     * @brief Initializes an ambient light
     *
     * @param process   The process this light is attached to.
     * @param color     The color of the light.
     * @param intensity The intensity of the light.
     */
    explicit AmbientLight(Process *process, const vec3 &color = vec3(1), float intensity = 0.2f):
        Light(process, LightType::AmbientLight),
        color(color),
        intensity(intensity)
    {
        this->name = "AmbientLight";
    }

    /// @brief Retrieves the light color.
    [[nodiscard]] const vec3 &getColor() const { return color; }

    /// @brief Changes the light color.
    void setColor(const vec3 &value) { AmbientLight::color = value; }

    /// @brief Retrieves the light intensity.
    [[nodiscard]] float getIntensity() const { return intensity; }

    /// @brief Changes the light intensity.
    void setIntensity(float value) { AmbientLight::intensity = value; }

protected:

    /// The light color
    vec3 color;

    /// The light intensity
    float intensity;

};


