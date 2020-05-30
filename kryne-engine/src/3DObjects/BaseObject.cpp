//
// Created by max on 22/04/19.
//

#include <kryne-engine/Core/AdditionalParameters.h>
#include "kryne-engine/3DObjects/BaseObject.h"

BaseObject::BaseObject(Shader *shader, VertexArray *vertexArray)
{
    this->shader = shader;
    this->vertexArray = vertexArray;
    supportsLighting = false;
    maximumSupportedPointLights = 0;
}


void BaseObject::setShader(Shader *shader)
{
    BaseObject::shader = shader;
}


void BaseObject::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params)
{
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    shader->setMat3("normalMat", glm::inverse(glm::transpose(glm::mat3(model))));
    vertexArray->execute();
}


Shader *BaseObject::getShader() const
{
    return shader;
}


bool BaseObject::isSupportingLighting() const {
    return supportsLighting;
}


int BaseObject::getMaximumSupportedPointLights() const
{
    return maximumSupportedPointLights;
}


BaseObjectShadowType BaseObject::getShadowCasting() const
{
    return shadowCasting;
}


void BaseObject::shapeDraw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params)
{
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    shader->setMat3("normalMat", glm::inverse(glm::transpose(glm::mat3(model))));
    vertexArray->execute();
}
