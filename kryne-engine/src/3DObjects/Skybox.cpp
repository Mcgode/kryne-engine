//
// Created by max on 02/08/2019.
//

#include "kryne-engine/3DObjects/Skybox.h"


Skybox::Skybox(const std::string &cubemapDirectoryName, const std::string &fileExtension)
{
    std::vector<glm::vec3> vertices, normals, tangents;
    std::vector<glm::vec2> textureCoordinates;

    generateCubeShape(1.0, 1.0, 1.0, &vertices, &normals, &textureCoordinates, &tangents);
    auto va = new VertexArray(&vertices);
    va->add_array(&normals);
    va->add_array(&textureCoordinates);
    va->add_array(&tangents);
    this->vertexArray = va;

    this->shader = new Shader("Skybox/Skybox");

//    this->cubemap = new CubeTexture("Resources/Textures/" + cubemapDirectoryName, fileExtension);
    throw "Fix needed";
}


void Skybox::draw(glm::mat4 projection, glm::mat4 view)
{
    glCullFace(GL_FRONT);
    shader->use();

//    cubemap->setTexture(shader, "cubemap");
    throw "Fix needed";

    shader->setUniform("projectionMatrix", projection);
    shader->setUniform("viewMatrix", view);
    vertexArray->execute(GL_TRIANGLES);
    Shader::resetUse();
    glCullFace(GL_BACK);
}


void Skybox::setLightDirection(glm::vec3 direction)
{
    shader->use();
    shader->setUniform("sunDirection", direction);
    Shader::resetUse();
}
