//
// Created by max on 21/05/19.
//

#ifndef INC_3D_DEMOS_DIRECTIONALLIGHT_H
#define INC_3D_DEMOS_DIRECTIONALLIGHT_H

#include <glm/vec3.hpp>
#include <common/Shader.h>


class DirectionalLight {

public:

    explicit DirectionalLight(glm::vec3 direction,
                              glm::vec3 diffuseColor = glm::vec3(1),
                              glm::vec3 *ambientColor = nullptr,
                              glm::vec3 *specularColor = nullptr);

    void shaderSet(Shader *shader, std::string uniformName);

private:

    glm::vec3 direction{};

    glm::vec3 ambientColor{};
    glm::vec3 diffuseColor{};
    glm::vec3 specularColor{};
};


#endif //INC_3D_DEMOS_DIRECTIONALLIGHT_H
