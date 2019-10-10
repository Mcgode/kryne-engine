//
// Created by max on 07/10/2019.
//

#ifndef INC_3D_DEMOS_MAIN_RENDERER_H
#define INC_3D_DEMOS_MAIN_RENDERER_H

#include <common/Light/DirectionalLight.h>
#include <common/Light/PointLight.h>
#include <common/Camera/Camera.h>
#include <common/3DObjects/Skybox.h>
#include <common/Rendering/RenderPass.h>


class MainRenderer : public RenderPass {

public:


    MainRenderer(Camera *camera, unsigned int width, unsigned int height);


    void render(Window *window, std::vector<HierarchicalNode *> *rootNodes, AdditionalParameters *params) override {
        std::cerr << "You must use the " << std::endl;
        exit(EXIT_FAILURE);
    }


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
    Camera *updateCamera(Camera *newCamera);


    void setSkybox(Skybox *skybox, SkyboxDrawOrder drawOrder, DirectionalLight *directionalLight);


    [[nodiscard]] Camera *getCamera() const;


    [[nodiscard]] Skybox *getSkybox() const;


    virtual ~MainRenderer();


private:

    /// The current camera for the scene
    Camera *camera;

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

#endif //INC_3D_DEMOS_MAIN_RENDERER_H
