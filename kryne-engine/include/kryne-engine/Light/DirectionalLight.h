/**
 * @file
 * @author Max Godefroy
 * @date 21/05/19
 */

#ifndef INC_KRYNE_ENGINE_DIRECTIONALLIGHT_H
#define INC_KRYNE_ENGINE_DIRECTIONALLIGHT_H

#include <glm/vec3.hpp>
#include <vector>
#include <kryne-engine/Material/Shader.h>

/**
 * A wrapper for a light that shines in a given direction, independently of the distance
 */
class DirectionalLight {

public:

    /**
     * A wrapper for a light that shines in a given direction, independently of the distance
     * @param direction     The vector representing the direction of the light
     * @param diffuseColor  The diffuse color of the light, defaults to white (#FFFFFF)
     * @param ambientColor  The ambient color of the light, defaults to 0.1 * diffuseColor
     * @param specularColor The specular color of the light, defaults to the diffuse color
     */
    explicit DirectionalLight(glm::vec3 direction,
                              glm::vec3 diffuseColor = glm::vec3(1),
                              glm::vec3 *ambientColor = nullptr,
                              glm::vec3 *specularColor = nullptr);


    /**
     * Sets up all the light properties in the fragment shader
     * @param shader        The shader program where you want to apply the light properties
     * @param uniformName   The name of the uniform compiling all the light properties
     */
    void shaderSet(Shader *shader, std::string uniformName);


    [[nodiscard]] const glm::vec3 &getDirection() const;


    [[nodiscard]] bool isCastingShadow() const;


    [[nodiscard]] uint16_t getShadowResolution() const;


    [[nodiscard]] std::vector<double> getShadowCastRadii() const;


    void setCastsShadow(uint16_t resolution, double radius);


    void setCastsShadowCascaded(uint16_t resolution, std::vector<double> radii);

private:

    /// The vector of the light direction
    glm::vec3 direction{};

    glm::vec3 ambientColor{}; ///< The ambient color of the light
    glm::vec3 diffuseColor{}; ///< The diffuse color of the light
    glm::vec3 specularColor{}; ///< The specular color of the light

    bool castsShadow = false; ///< Set to true to have this light cast shadows
    uint16_t shadowResolution{}; ///< The pixel resolution for the shadow map
    std::vector<double> shadowCastRadii{}; ///< Radius of the sphere which content will be rendered on the shadow map
};


#endif //INC_KRYNE_ENGINE_DIRECTIONALLIGHT_H
