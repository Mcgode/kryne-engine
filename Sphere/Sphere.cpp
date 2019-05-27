//
// Created by max on 21/05/19.
//

#include "Sphere.h"

Sphere::Sphere() {
    auto shader = new Shader("Sphere/NormalMap");

    std::vector<glm::vec3> vertices, normals, tangents;
    std::vector<glm::vec2> textureCoordinates;

    generateSphereShape(1.5, 50, 25, &vertices, &normals, &textureCoordinates, &tangents);
    auto va = new VertexArray(&vertices);
    va->add_array(&normals);
    va->add_array(&textureCoordinates);
    va->add_array(&tangents);

    this->shader = shader;
    this->vertexArray = va;
    this->supportsLighting = true;
    this->maximumSupportedPointLights = 4;
    this->castsHardShadows = true;

    this->diffuseTexture = new Texture2D("Textures/brickwall.jpg");
    this->normalMapTexture = new Texture2D("Textures/brickwall_normal.jpg");
}


void Sphere::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params)
{
    shader->setFloat("material.shininess", 12.0f);
    shader->setVec3("material.ambient", glm::vec3(0.07));
    shader->setVec3("material.diffuse", glm::vec3(1.0));
    shader->setVec3("material.specular", glm::vec3(0.8));
    diffuseTexture->setTexture(shader, 0, "material.diffuseMap");
    normalMapTexture->setTexture(shader, 1, "material.normalMap");
    BaseObject::draw(projection, view, model, params);
}
