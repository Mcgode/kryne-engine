/**
 * @file
 * @author Max Godefroy
 * @date 11/10/2019
 */

#ifndef INC_KRYNE_ENGINE_DIRECTIONAL_SHADOW_MAP_RENDERING_H
#define INC_KRYNE_ENGINE_DIRECTIONAL_SHADOW_MAP_RENDERING_H


#include <kryne-engine/Rendering/OldRenderPass.h>
#include <kryne-engine/Light/OldDirectionalLight.h>

class DirectionalShadowMapRendering: public OldRenderPass {

public:

    explicit DirectionalShadowMapRendering(OldDirectionalLight *directionalLight,
                                           OldCamera *mainCamera);


    void render(Window *window, std::vector<HierarchicalNode *> *rootNodes, AdditionalParameters *params) override;


    void drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;


    [[nodiscard]] std::vector<GLuint> getShadowMaps() const;


    [[nodiscard]] std::vector<glm::mat4> getLightSpaceMatrices();


    void updateCamera(OldCamera *newCamera);


private:

    OldDirectionalLight *light;
    OldCamera *camera;

    std::vector<GLuint> fbos;
    std::vector<GLuint> shadowMaps;

    Shader *shadowMapShader;
    glm::mat4 projection{};
    uint8_t current_index = 0;
    uint16_t resolution;


    glm::mat4 computeMatrices();

};


#endif //INC_KRYNE_ENGINE_DIRECTIONAL_SHADOW_MAP_RENDERING_H
