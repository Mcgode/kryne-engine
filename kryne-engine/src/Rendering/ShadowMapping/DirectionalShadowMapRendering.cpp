//
// Created by max on 11/10/2019.
//

#include "kryne-engine/Rendering/ShadowMapping/DirectionalShadowMapRendering.h"

DirectionalShadowMapRendering::DirectionalShadowMapRendering(DirectionalLight *directionalLight,
                                                             Camera *mainCamera)
{
    this->light = directionalLight;
    auto radii = directionalLight->getShadowCastRadii();

    this->camera = mainCamera;

    this->fbos = vector<GLuint>(radii.size());
    this->shadowMaps = vector<GLuint>(radii.size());

    glGenFramebuffers(radii.size(), this->fbos.data());
    glGenTextures(radii.size(), this->shadowMaps.data());

    this->resolution = directionalLight->getShadowResolution();

    for (this->current_index = 0; this->current_index < radii.size(); this->current_index++) {
        glBindTexture(GL_TEXTURE_2D, this->shadowMaps[this->current_index]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     this->resolution, this->resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, this->fbos[this->current_index]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                               this->shadowMaps[this->current_index], 0);
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
    }

    this->shadowMapShader = new Shader("engine/shadowMap");
}


void DirectionalShadowMapRendering::render(Window *window,
                                           std::vector<HierarchicalNode *> *rootNodes,
                                           AdditionalParameters *params)
{
    glViewport(0, 0, this->resolution, this->resolution);

    for (this->current_index = 0; this->current_index < this->fbos.size(); this->current_index++) {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbos[this->current_index]);
        glClear(GL_DEPTH_BUFFER_BIT);

        auto view = this->computeMatrices();

        for (HierarchicalNode *node: *rootNodes) {
            node->draw(this, view, glm::mat4(1.0), params);
        }
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

        this->shadowMapShader->setMat4("lightSpaceMatrix", this->projection * view);

        obj->draw(glm::mat4(this->projection), view, model, params);

        Shader::resetUse();

        obj->setShader(normalShader);
    }
}


std::vector<GLuint> DirectionalShadowMapRendering::getShadowMaps() const
{
    return this->shadowMaps;
}

vector<glm::mat4> DirectionalShadowMapRendering::getLightSpaceMatrices() {
    std::vector<glm::mat4> result;

    for (this->current_index = 0; this->current_index < this->fbos.size(); this->current_index++) {
        auto view = this->computeMatrices();
        result.push_back(glm::mat4(this->projection) * glm::mat4(view));
    }

    return result;
}


glm::mat4 DirectionalShadowMapRendering::computeMatrices()
{
    double radius = this->light->getShadowCastRadii()[this->current_index];
    this->projection = glm::ortho(-radius, radius, -radius, radius, 0.0, 2.0 * radius);

    glm::vec3 upDirection = glm::vec3(0., 1., 0.);
    glm::vec3 dir = this->light->getDirection();
    if (abs(glm::dot(upDirection, dir)) == 1.)
        upDirection = glm::vec3(1., 0., 0.);
    else
        upDirection = glm::normalize(glm::cross(upDirection, dir));
    glm::vec3 center = this->camera->getCurrentPosition();

    return glm::lookAt(center - dir * (float) radius, center, upDirection);
}


void DirectionalShadowMapRendering::updateCamera(Camera *newCamera)
{
    this->camera = newCamera;
}
