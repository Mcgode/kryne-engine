//
// Created by max on 11/10/2019.
//

#ifndef INC_3D_DEMOS_DIRECTIONAL_SHADOW_MAP_RENDERING_H
#define INC_3D_DEMOS_DIRECTIONAL_SHADOW_MAP_RENDERING_H


#include <kryne-engine/Rendering/RenderPass.h>
#include <kryne-engine/Light/DirectionalLight.h>

class DirectionalShadowMapRendering: public RenderPass {

public:

    explicit DirectionalShadowMapRendering(DirectionalLight *directionalLight);


    void render(Window *window, std::vector<HierarchicalNode *> *rootNodes, AdditionalParameters *params) override;


    void drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;


    [[nodiscard]] GLuint getShadowMap() const;


    [[nodiscard]] glm::mat4 getLightSpaceMatrix() const;


private:

    GLuint fbo = 0;
    GLuint shadowMap = 0;

    Shader *shadowMapShader;

    glm::mat4 projection{};
    glm::mat4 view{};

    uint16_t resolution;

};


#endif //INC_3D_DEMOS_DIRECTIONAL_SHADOW_MAP_RENDERING_H
