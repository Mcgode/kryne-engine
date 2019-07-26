//
// Created by max on 26/07/2019.
//

#include <common/GeometricShapes.h>
#include "Floor.h"

Floor::Floor(const std::string &textureName)
{
    this->shader = new Shader("Quad/NormalMapping");

    std::vector<glm::vec3> vertices, normals, tangents;
    std::vector<glm::vec2> textureCoordinates;

    generateQuadShape(10.0, &vertices, &normals, &textureCoordinates, &tangents);
    auto va = new VertexArray(&vertices);
    va->add_array(&normals);
    va->add_array(&textureCoordinates);
    va->add_array(&tangents);
    this->vertexArray = va;

    this->diffuseTexture = Texture2D::getTexture(textureName, "diff");
    this->normalMapTexture = Texture2D::getTexture(textureName, "norm");

    supportsLighting = true;
    maximumSupportedPointLights = 4;

}

void Floor::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) {
    shader->setFloat("material.shininess", 64.0f);
    shader->setVec3("material.ambient", glm::vec3(0.07));
    shader->setVec3("material.diffuse", glm::vec3(1.0));
    shader->setVec3("material.specular", glm::vec3(1.6));
    this->diffuseTexture->setTexture(shader, 0, "material.diffuseMap");
    this->normalMapTexture->setTexture(shader, 1, "material.normalMap");
    BaseObject::draw(projection, view, model, params);
}


Floor::~Floor()
{
    delete this->shader;
    delete this->vertexArray;
    delete this->diffuseTexture;
    delete this->normalMapTexture;
}
