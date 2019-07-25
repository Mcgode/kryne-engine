//
// Created by max on 09/06/19.
//

#include "SphereBasic.h"

SphereBasic::SphereBasic()
{
    initialize(new Shader("Sphere/Lighting"));
}

void SphereBasic::initialize(Shader *shader)
{
    std::vector<glm::vec3> vertices, normals, tangents;
    std::vector<glm::vec2> textureCoordinates;

    generateSphereShape(1.5, 100, 50, &vertices, &normals, &textureCoordinates, &tangents);
//    generateQuadShape(3.0, &vertices, &normals, &textureCoordinates, &tangents);
    auto va = new VertexArray(&vertices);
    va->add_array(&normals);
    va->add_array(&textureCoordinates);
    va->add_array(&tangents);

    this->shader = shader;
    this->vertexArray = va;
    this->supportsLighting = true;
    this->maximumSupportedPointLights = 4;
    this->castsHardShadows = true;
}

void SphereBasic::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) {
    shader->setVec3("color", glm::vec3(1, 0, 0));
    BaseObject::draw(projection, view, model, params);
}
