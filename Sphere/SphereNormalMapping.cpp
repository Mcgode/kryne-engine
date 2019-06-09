//
// Created by max on 21/05/19.
//

#include "SphereNormalMapping.h"

SphereNormalMapping::SphereNormalMapping() {
    this->shader = new Shader("Sphere/NormalMap");
    this->normalMapTexture = new Texture2D("Textures/brickwall_normal.jpg");
}


void SphereNormalMapping::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params)
{
    normalMapTexture->setTexture(shader, 1, "material.normalMap");
    SphereTexture::draw(projection, view, model, params);
}
