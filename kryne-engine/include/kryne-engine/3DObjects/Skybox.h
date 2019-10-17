//
// Created by max on 02/08/2019.
//

#ifndef INC_3D_DEMOS_SKYBOX_H
#define INC_3D_DEMOS_SKYBOX_H

#include <string>

#include <kryne-engine/Shader.h>
#include <kryne-engine/TextureCubeMap.h>
#include <kryne-engine/VertexArray.h>
#include <kryne-engine/GeometricShapes.h>
#include <kryne-engine/enums/SkyboxDrawOrder.h>

class Skybox {

public:

    Skybox(const std::string &cubemapDirectoryName, const std::string &fileExtension);

    void draw(glm::mat4 projection, glm::mat4 view);

    void setLightDirection(glm::vec3 direction);

private:

    VertexArray *vertexArray{};
    Shader *shader{};
    TextureCubeMap *cubemap{};

};


#endif //INC_3D_DEMOS_SKYBOX_H