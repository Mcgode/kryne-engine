//
// Created by max on 29/07/2019.
//

#include "Model.h"

Model::Model(const string &model, const string &fallbackTexture)
{
    this->data = ModelLoader::loadModel(model);
    auto mesh = data[0];

    this->vertexArray = mesh->getVertexArray();
    this->shader = new Shader("Quad/NormalMapping");
    this->supportsLighting = true;

    this->diffuseMap = mesh->getDiffuseMap() ? mesh->getDiffuseMap() : Texture2D::getTexture(fallbackTexture);
    this->normalMap = mesh->getNormalMap() ? mesh->getNormalMap() : Texture2D::getTexture(fallbackTexture, "norm");
}

void Model::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) {
    shader->setFloat("material.shininess", 2.0f);
    shader->setVec3("material.ambient", glm::vec3(0.15));
    shader->setVec3("material.diffuse", glm::vec3(1.0));
    shader->setVec3("material.specular", glm::vec3(0.4));
    this->diffuseMap->setTexture(shader, "material.diffuseMap");
    this->normalMap->setTexture(shader, "material.normalMap");
    BaseObject::draw(projection, view, model, params);
}
