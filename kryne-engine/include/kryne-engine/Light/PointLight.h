/**
 * @file
 * @author Max Godefroy
 * @date 21/05/19
 */

#ifndef INC_3D_DEMOS_POINTLIGHT_H
#define INC_3D_DEMOS_POINTLIGHT_H

#include <glm/vec3.hpp>
#include <kryne-engine/Material/Shader.h>
#include <vector>
#include <algorithm>


/**
 * A wrapper for a light that shines from a provided position, with a potential attenuation
 */
class PointLight {

public:

    /**
     * A wrapper for a light that shines from a provided position, with a potential attenuation
     * @param direction     The vector representing the direction of the light
     * @param diffuseColor  The diffuse color of the light, defaults to white (#FFFFFF)
     * @param ambientColor  The ambient color of the light, defaults to 0.1 * diffuseColor
     * @param specularColor The specular color of the light, defaults to the diffuse color
     */
    explicit PointLight(glm::vec3 position,
                        glm::vec3 diffuseColor = glm::vec3(1),
                        glm::vec3 *ambientColor = nullptr,
                        glm::vec3 *specularColor = nullptr);

    /**
     * Sets the attenuation parameters
     * @param constant  The constant attenuation of the light
     * @param linear    The linear attenuation of the light
     * @param quadratic The quadratic attenuation of the light
     */
    void setAttenuation(float constant, float linear, float quadratic);

    /**
     * Sets up all the light properties in the fragment shader
     * @param shader        The shader program where you want to apply the light properties
     * @param uniformName   The name of the uniform compiling all the light properties
     */
    void shaderSet(Shader *shader, std::string uniformName);

    /**
     * Sets up all the light properties in the fragment shader at a certain index in an array
     * @param shader            The shader program where you want to apply the light properties
     * @param uniformArrayName  The name of the uniform array
     * @param index             The index of the light properties in the array
     */
    void shaderSetArray(Shader *shader, std::string uniformArrayName, size_t index);

    /**
     * Retrieves a maximum of points closest to a given position
     * @param pointLightList    The list of point lights to choose from
     * @param position          The position to which the points must be the closest possible
     * @param maximumAmount     The limit amount of point lights to retrieve
     * @return A vector containing the n (with n <= maximumAmount) closest point lights to the provided position from
     *         the input vector
     */
    static std::vector<PointLight*> getClosestPointLights(const std::vector<PointLight*>& pointLightList,
                                                          glm::vec3 position,
                                                          size_t maximumAmount);


    [[nodiscard]] bool isCastingShadow() const;


private:

    /// The position of the point light
    glm::vec3 position{};

    glm::vec3 ambientColor{}; ///< The ambient color of the light
    glm::vec3 diffuseColor{}; ///< The diffuse color of the light
    glm::vec3 specularColor{}; ///< The specular color of the light

    float constantAttenuation{}; ///< The constant attenuation of the light
    float linearAttenuation{}; ///< The linear attenuation of the light
    float quadraticAttenuation{}; ///< The quadratic attenuation of the light

    bool castsShadow = false; ///< Set to true to have this light cast shadows
};


#endif //INC_3D_DEMOS_POINTLIGHT_H
