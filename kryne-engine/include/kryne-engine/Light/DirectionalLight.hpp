/**
 * @file
 * @author Max Godefroy
 * @date 06/04/2021.
 */

#pragma once


#include "Light.hpp"
#include <kryne-engine/Rendering/Framebuffer.hpp>


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


// =====================
// Shadows
// =====================

friend class LightingRegistrySystem;
friend class ShadowMappingProcess;
friend class DirectionalLightShadowCameraComponent;

public:

    /// @brief Retrieves whether this light should cast shadows or not.
    [[nodiscard]] bool isCastShadow() const { return castShadow; }

    /// @brief Updates whether this light should cast shadows or not.
    void setCastShadow(bool val) { DirectionalLight::castShadow = val; }

    /// @brief Retrieves the camera frustum far distance after which the shadows shouldn't be computed.
    [[nodiscard]] float getMaxShadowDistance() const { return maxShadowDistance; }

    /// @brief Updates the camera frustum far distance after which the shadows shouldn't be computed.
    void setMaxShadowDistance(float value) { DirectionalLight::maxShadowDistance = value; }

    /// @brief Retrieves the minimum depth of the shadow frustum.
    [[nodiscard]] float getMinShadowDepth() const { return minShadowDepth; }

    /// @brief Updates the minimum depth of the shadow frustum.
    void setMinShadowDepth(float value) { DirectionalLight::minShadowDepth = value; }

    /// @brief Retrieves the amount of cascaded shadow maps for this light.
    [[nodiscard]] const uint8 &getCascadedShadowMaps() const { return cascadedShadowMaps; }

    /**
     * @brief Updates the amount of cascaded shadow maps for this light.
     *
     * @details
     * The engine supports up to 4 cascaded shadow maps at this time, so the value will automatically be clamped between
     * 1 and 4.
     *
     * For cascading, the camera frustum depth is divided into 2^n-1 segments, with individual shadow maps handling 2^i
     * segments. It means that each shadow map handles half the depth of the next shadow map.
     */
    void setCascadedShadowMaps(const uint8 &value)
    {
        DirectionalLight::cascadedShadowMaps = glm::clamp(value, (uint8) 1, (uint8) 4);
    }

protected:

    /// @brief A struct for storing the relevant shadow map data
    struct ShadowMapData {

        /// The camera for the shadow map
        unique_ptr<Camera> shadowCamera;

        /// The shadow map framebuffer
        unique_ptr<Framebuffer> shadowFramebuffer;

        ShadowMapData(unique_ptr<Camera> shadowCamera,
                      unique_ptr<Framebuffer> shadowFramebuffer) :
                          shadowCamera(move(shadowCamera)),
                          shadowFramebuffer(move(shadowFramebuffer)) {}

    };

protected:

    /// Toggle whether this light should cast shadows or not.
    bool castShadow = false;

    /// The far distance after which the shadows shouldn't be computed.
    float maxShadowDistance = 1e3;

    /// The minimum depth of the shadow frustum.
    float minShadowDepth = 1e2;

    /// The amount of cascaded shadow maps. The engine handles up to 4 of them.
    uint8 cascadedShadowMaps = 1;

    /// The shadow map data of the light, for a maximum of 4 cascaded shadow maps.
    unique_ptr<ShadowMapData> shadowMapData[4];

};


