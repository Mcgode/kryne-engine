/**
 * @file
 * @author Max Godefroy
 * @date 11/10/2019
 */

#ifndef INC_3D_DEMOS_DIRECTIONAL_SHADOW_MAP_RENDERING_H
#define INC_3D_DEMOS_DIRECTIONAL_SHADOW_MAP_RENDERING_H


#include <kryne-engine/Rendering/RenderPass.h>
#include <kryne-engine/Light/DirectionalLight.h>

class DirectionalShadowMapRendering: public RenderPass {

public:

    explicit DirectionalShadowMapRendering(DirectionalLight *directionalLight,
                                           OldCamera *mainCamera);


    void render(Window *window, std::vector<HierarchicalNode *> *rootNodes, AdditionalParameters *params) override;


    void drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;


    [[nodiscard]] std::vector<GLuint> getShadowMaps() const;


    [[nodiscard]] std::vector<glm::mat4> getLightSpaceMatrices();


    void updateCamera(OldCamera *newCamera);


private:

    DirectionalLight *light;
    OldCamera *camera;

    std::vector<GLuint> fbos;
    std::vector<GLuint> shadowMaps;

    Shader *shadowMapShader;
    glm::mat4 projection{};
    uint8_t current_index = 0;
    uint16_t resolution;


    glm::mat4 computeMatrices();

};


#endif //INC_3D_DEMOS_DIRECTIONAL_SHADOW_MAP_RENDERING_H
