//
// Created by max on 21/05/19.
//

#ifndef INC_3D_DEMOS_POINTLIGHT_H
#define INC_3D_DEMOS_POINTLIGHT_H

#include <glm/vec3.hpp>
#include <common/Shader.h>
#include <vector>
#include <algorithm>

class PointLight {

public:

    explicit PointLight(glm::vec3 position,
                        glm::vec3 diffuseColor = glm::vec3(1),
                        glm::vec3 *ambientColor = nullptr,
                        glm::vec3 *specularColor = nullptr);

    void setAttenuation(float constant, float linear, float quadratic);

    void shaderSet(Shader *shader, std::string uniformName);

    void shaderSetArray(Shader *shader, std::string uniformArrayName, size_t index);

    static std::vector<PointLight*> getClosestPointLights(const std::vector<PointLight*>& pointLightList,
                                                          glm::vec3 position,
                                                          size_t maximumAmount);

private:

    glm::vec3 position{};

    glm::vec3 ambientColor{};
    glm::vec3 diffuseColor{};
    glm::vec3 specularColor{};

    float constantAttenuation{};
    float linearAttenuation{};
    float quadraticAttenuation{};
};


#endif //INC_3D_DEMOS_POINTLIGHT_H
