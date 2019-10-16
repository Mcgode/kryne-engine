//
// Created by max on 11/10/2019.
//

#include "kryne-engine/Rendering/ShadowMapping/DirectionalShadowMapRendering.h"

DirectionalShadowMapRendering::DirectionalShadowMapRendering(DirectionalLight *directionalLight)
{
    glGenFramebuffers(1, &this->fbo);
    glGenTextures(1, &this->shadowMap);

    this->resolution = directionalLight->getShadowResolution();

    glBindTexture(GL_TEXTURE_2D, this->shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 this->resolution, this->resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

//    GLuint rbo;
//    glGenRenderbuffers(1, &rbo);
//    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, directionalLight->getShadowResolution(), directionalLight->getShadowResolution());
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cerr << "Shadow map framebuffer initialization failed" << endl;
        exit(EXIT_FAILURE);
    }

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
    glViewport(0, 0, this->resolution, this->resolution);

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    for (HierarchicalNode *node: *rootNodes) {
        node->draw(this, this->view, glm::mat4(1.0), params);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void DirectionalShadowMapRendering::drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model,
                                                AdditionalParameters *params)
{
    if (obj->getShadowCasting() == HARD_SHADOW) {
        auto normalShader = obj->getShader();
        obj->setShader(this->shadowMapShader);

        obj->getShader()->use();

        this->shadowMapShader->setMat4("lightSpaceMatrix", this->getLightSpaceMatrix());

        obj->draw(glm::mat4(this->projection), view, model, params);

        Shader::resetUse();

        obj->setShader(normalShader);
    }
}


GLuint DirectionalShadowMapRendering::getShadowMap() const
{
    return this->shadowMap;
}

glm::mat4 DirectionalShadowMapRendering::getLightSpaceMatrix() const {
    return glm::mat4(projection) * glm::mat4(view);
}
