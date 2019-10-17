//
// Created by max on 02/08/2019.
//

#include <kryne-engine/Core/GeometricShapes.h>
#include "StainedGlass.h"

StainedGlass::StainedGlass()
{
    this->shader = new Shader("AlphaTexture/Basic");
    this->supportsLighting = true;
    this->shadowCasting = NO_SHADOW;
    this->maximumSupportedPointLights = 4;

    vector<glm::vec3> positions, normals, tangents;
    vector<glm::vec2> textureCoordinates;
    generateDoubleSidedQuadShape(2.0, &positions, &normals, &textureCoordinates, &tangents);

    auto va = new VertexArray(&positions);
    va->add_array(&normals);
    va->add_array(&textureCoordinates);
    va->add_array(&tangents);

    this->vertexArray = va;

    this->texture = new Texture2D("Resources/Textures/stainedGlass.png");
}


void StainedGlass::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) {
    shader->setFloat("material.shininess", 128.0f);
    shader->setVec3("material.ambient", glm::vec3(0.08));
    shader->setVec3("material.diffuse", glm::vec3(1.0));
    shader->setVec3("material.specular", glm::vec3(5.2));
    texture->setTexture(shader, "material.diffuseMap");
    BaseObject::draw(projection, view, model, params);
}
