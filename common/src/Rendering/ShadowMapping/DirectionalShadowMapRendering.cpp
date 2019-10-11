//
// Created by max on 11/10/2019.
//

#include "common/Rendering/ShadowMapping/DirectionalShadowMapRendering.h"

DirectionalShadowMapRendering::DirectionalShadowMapRendering(DirectionalLight *directionalLight)
{
    glGenFramebuffers(1, &this->fbo);
    glGenTextures(1, &this->shadowMap);

    glBindTexture(GL_TEXTURE_2D, this->shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, directionalLight->getShadowResolution(),
            directionalLight->getShadowResolution(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    this->shadowMapShader = new Shader("engine/shadowMap");

    double radius = directionalLight->getShadowCastRadius();
    this->projection = glm::ortho(-radius, radius, -radius, radius, 0.0, 2.0 * radius);

    glm::vec3 upDirection = glm::vec3(0., 1., 0.);
    glm::vec3 dir = directionalLight->getDirection();
    if (abs(glm::dot(upDirection, dir)) == 1.)
        upDirection = glm::vec3(1., 0., 0.);
    else
        upDirection = glm::normalize(glm::cross(upDirection, dir));
    glm::vec3 center = directionalLight->getShadowCastCenter();

    this->view = glm::lookAt(center - dir * (float) radius, center, upDirection);
}


void DirectionalShadowMapRendering::render(Window *window, std::vector<HierarchicalNode *> *rootNodes,
                                           AdditionalParameters *params)
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    for (HierarchicalNode *node: *rootNodes) {
        node->draw(this, this->view, glm::mat4(1.0), params);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void DirectionalShadowMapRendering::drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model,
                                                AdditionalParameters *params)
{
    this->shadowMapShader->use();
    obj->draw(this->projection, view, model, params);
    Shader::resetUse();
}


GLuint DirectionalShadowMapRendering::getShadowMap() const
{
    return this->shadowMap;
}
