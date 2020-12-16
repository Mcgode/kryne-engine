/**
 * @file
 * @author Max Godefroy
 * @date 07/10/2019
 */

#ifndef INC_KRYNE_ENGINE_MAIN_RENDERER_H
#define INC_KRYNE_ENGINE_MAIN_RENDERER_H

#include <kryne-engine/Light/LightingRegistry.h>
#include <kryne-engine/Camera/OldCamera.h>
#include <kryne-engine/3DObjects/Skybox.h>
#include <kryne-engine/Rendering/RenderPass.h>


class MainRenderer : public RenderPass {

public:


    MainRenderer(OldCamera *camera, unsigned int width, unsigned int height);


    void render(Window *window, std::vector<HierarchicalNode *> *rootNodes, AdditionalParameters *params) override;


    /**
     * The method for the rendering the current pass
     * @param window            The current rendering window.
     * @param rootNodes         The nodes to render.
     * @param directionalLight  The directional light for the scene
     * @param pointLights       The point lights for the scene
     * @param params            Additional parameters to pass down during the rendering.
     */
    void renderMain(Window *window, std::vector<HierarchicalNode *> *rootNodes, DirectionalLight *directionalLight,
                    std::vector<PointLight *> *pointLights, AdditionalParameters *params);


    void drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;


    /**
     * Changes the current camera
     * @param newCamera     The new camera you want to use in the scene
     */
    OldCamera *updateCamera(OldCamera *newCamera);


    void setSkybox(Skybox *skybox, SkyboxDrawOrder drawOrder, DirectionalLight *directionalLight);


    [[nodiscard]] OldCamera *getCamera() const;


    [[nodiscard]] Skybox *getSkybox() const;


    virtual ~MainRenderer();


private:

    /// The current camera for the scene
    OldCamera *camera;

    /// The projection matrix for the scene
    glm::mat4 projection{};

    /// The directional light for the scene
    DirectionalLight *directionalLight{};

    /// The point lights of the scene
    std::vector<PointLight *> *pointLights;

    Skybox *skybox = nullptr;
    SkyboxDrawOrder skyboxDrawOrder = SKYBOX_NO_DRAW;
};

#else

class MainRenderer;

#endif //INC_KRYNE_ENGINE_MAIN_RENDERER_H
