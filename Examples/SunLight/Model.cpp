//
// Created by max on 29/07/2019.
//

#include "Model.h"

Model::Model(const string &model, const string &fallbackTexture, const Scene *scene, DirectionalLight *light)
{
    this->data = ModelLoader::loadModel(model);
    auto mesh = data[0];

    this->handler = scene->getShadowMapHandler();
    this->light = light;

    this->vertexArray = mesh->getVertexArray();
    this->shader = new Shader("Quad/ShadowMap");
    this->supportsLighting = true;

    shadowCasting = HARD_SHADOW;

    this->diffuseMap = mesh->getDiffuseMap() ? mesh->getDiffuseMap() : Texture2D::getTexture(fallbackTexture);
    this->normalMap = mesh->getNormalMap() ? mesh->getNormalMap() : Texture2D::getTexture(fallbackTexture, "norm");
}

void Model::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) {
    shader->setFloat("material.shininess", 2.0f);
    shader->setVec3("material.ambient", glm::vec3(0.15));
    shader->setVec3("material.diffuse", glm::vec3(1.0));
    shader->setVec3("material.specular", glm::vec3(0.4));

    auto lsms = handler->getLightSpaceMatrices(light);
    auto sms = handler->getDirectionalShadowMaps(light);
    for (int i = 0; i < sms.size(); i++) {
        Texture2D::textureSet(sms[i], shader, "directionalShadowMaps[" + std::to_string(i) + "].shadowMap");
        shader->setMat4("directionalShadowMaps[" + std::to_string(i) + "].lightSpaceMatrix", lsms[i]);
        shader->setFloat("directionalShadowMaps[" + std::to_string(i) + "].shadowBias", 0.01);
    }
    this->shader->setInt("amountOfShadowMaps", sms.size());

    this->diffuseMap->setTexture(shader, "material.diffuseMap");
    this->normalMap->setTexture(shader, "material.normalMap");
    BaseObject::draw(projection, view, model, params);
}
