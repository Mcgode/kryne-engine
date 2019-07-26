//
// Created by max on 21/05/19.
//

#include "SphereNormalMapping.h"

SphereNormalMapping::SphereNormalMapping(const std::string &textureDirName) : SphereTexture(textureDirName) {
    delete this->shader;
    this->shader = new Shader("Sphere/NormalMap");
    this->normalMapTexture = Texture2D::getTexture(textureDirName, "norm");
}


void SphereNormalMapping::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params)
{
    normalMapTexture->setTexture(shader, 1, "material.normalMap");
    SphereTexture::draw(projection, view, model, params);
}


SphereNormalMapping::~SphereNormalMapping()
{
//    SphereTexture::~SphereTexture();
    delete this->normalMapTexture;
}
