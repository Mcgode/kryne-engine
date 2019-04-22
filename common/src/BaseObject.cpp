//
// Created by max on 22/04/19.
//

#include "common/BaseObject.h"

BaseObject::BaseObject(Shader *shader, VertexArray *vertexArray)
{
    this->shader = shader;
    this->vertexArray = vertexArray;
    supportsLighting = false;
    maximumSupportedPointLights = 0;
    castsHardShadows = true;
}


void BaseObject::setShader(Shader *shader)
{
    BaseObject::shader = shader;
}


void BaseObject::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, std::map<std::string, void *> *params)
{
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    shader->setMat4("normalMat", glm::inverse(glm::transpose(glm::mat3(model))));
    vertexArray->execute();
}