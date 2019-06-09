//
// Created by max on 09/06/19.
//

#include "SphereTexture.h"


SphereTexture::SphereTexture()
{
    initialize(new Shader("Sphere/DiffuseMap"));

    this->diffuseTexture = new Texture2D("Textures/brickwall.jpg");
}


void SphereTexture::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params)
{
    shader->setFloat("material.shininess", 64.0f);
    shader->setVec3("material.ambient", glm::vec3(0.07));
    shader->setVec3("material.diffuse", glm::vec3(1.0));
    shader->setVec3("material.specular", glm::vec3(1.6));
    diffuseTexture->setTexture(shader, 0, "material.diffuseMap");
    SphereBasic::draw(projection, view, model, params);
}

