/**
 * @file
 * @author Max Godefroy
 * @date 22/04/19
 */

#ifndef INC_KRYNE_ENGINE_RENDERSCENE_H
#define INC_KRYNE_ENGINE_RENDERSCENE_H


#include <kryne-engine/Core/Window.h>
#include <kryne-engine/Rendering/MainRenderer.h>
#include <kryne-engine/Rendering/ShadowMapping/ShadowMapHandler.h>
#include <kryne-engine/Light/LightingRegistry.h>

/**
 * A class for handling a basic 3D scene
 */
class RenderScene {

public:

    /**
     * A class for handling a basic 3D scene
     * @param window        The window for the current process.
     * @param camera        The camera used for this scene
     */
    explicit RenderScene(Window *window, OldCamera *camera);

    /**
     * Changes the current camera
     * @param newCamera The new camera you want to use in the scene
     */
    OldCamera *updateCamera(OldCamera *newCamera);

    /**
     * Adds a new drawable root node to the scene
     * @param node The node to add to the scene
     */
    void addDrawable(HierarchicalNode *node);


    /**
     * The method for launching the rendering loop
     * @param params Additional parameters to pass down during the rendering
     */
    void renderLoop(AdditionalParameters *params);

    /**
     * Sets the directional light of the scene
     * @param light The directional light for the scene
     */
    void setDirectionalLight(DirectionalLight *light);

    /**
     * Adds a point light to scene
     * @param light The point light that you want to be added to the scene
     */
    void addPointLight(PointLight *light);

    OldCamera *getCamera() const;

    void setSkybox(Skybox *skybox, SkyboxDrawOrder drawOrder);

    [[nodiscard]] ShadowMapHandler *getShadowMapHandler() const;

    ~RenderScene();

private:

    /// The window where the rendering display will happen
    Window *window;

    /// All the drawable root nodes to render in the scene
    std::vector<HierarchicalNode *> rootNodes;

    /// The directional light for the scene
    shared_ptr<LightingRegistry> lightingRegistry;

    MainRenderer *mainRenderer{};

    ShadowMapHandler *shadowMapHandler;

};


#endif //INC_KRYNE_ENGINE_RENDERSCENE_H
