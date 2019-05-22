//
// Created by max on 21/05/19.
//

#include "Sphere.h"

Sphere::Sphere(Shader *shader) {
    std::vector<glm::vec3> vertices, normals, tangents;
    std::vector<glm::vec2> textureCoordinates;

    generateSphereShape(0.5, 50, 25, &vertices, &normals, &textureCoordinates, &tangents);
    auto va = new VertexArray(&vertices);
    va->add_array(&normals);
    va->add_array(&textureCoordinates);
    va->add_array(&tangents);

    this->shader = shader;
    this->vertexArray = va;
    this->supportsLighting = true;
}

void Sphere::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) {
    BaseObject::draw(projection, view, model, params);
}
