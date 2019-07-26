//
// Created by max on 09/06/19.
//

#include "SphereParallax.h"

SphereParallax::SphereParallax(const std::string& textureDirName) : SphereNormalMapping(textureDirName) {
    delete this->shader;
    this->displacementMap = Texture2D::getTexture(textureDirName, "disp");
    this->shader = new Shader("Sphere/Parallax");
}

void SphereParallax::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) {
    this->displacementMap->setTexture(this->shader, 2, "material.depthMap");
    SphereNormalMapping::draw(projection, view, model, params);
}


SphereParallax::~SphereParallax()
{
    delete this->displacementMap;
}
