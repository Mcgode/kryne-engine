//
// Created by max on 22/04/19.
//

#ifndef INC_3D_DEMOS_SCENE_H
#define INC_3D_DEMOS_SCENE_H


#include "common/Camera/Camera.h"
#include "Window.h"
#include "HierarchicalNode.h"
#include "Types.h"

/**
 * A class for handling a basic 3D scene
 */
class Scene {

public:

    /**
     * A class for handling a basic 3D scene
     * @param camera        The camera used for this scene
     * @param window_width  The width for the desktop window
     * @param window_height The height for the desktop window
     */
    explicit Scene(Camera *camera,
                   int window_width = 1280,
                   int window_height = 720);

    /**
     * Changes the current camera
     * @param newCamera The new camera you want to use in the scene
     */
    Camera *updateCamera(Camera *newCamera);

    /**
     * Adds a new drawable root node to the scene
     * @param node The node to add to the scene
     */
    void addDrawable(HierarchicalNode *node);


    /**
     * The method for the main rendering loop
     * @param params Additional parameters to pass down during the rendering
     */
    void mainRenderLoop(std::map<std::string, void *> *params = nullptr);

    /**
     * Draws an object in the scene.
     * @param obj        The object to draw
     * @param projection The projection transform matrix
     * @param view       The view transform matrix
     * @param model      The model transform matrix
     * @param params     The additional parameters to pass down to the object
     */
    void drawInScene(BaseObject *obj, glm::mat4 view, glm::mat4 model, AdditionalParams_t *params);

    /**
     * Runs the main loop until the window is asked to be closed
     * @param params The additional parameters to pass down to the loop
     */
    void runLoop(AdditionalParams_t *params = nullptr);

    ~Scene();

private:

    /// The window where the rendering display will happen
    Window *window;

    /// The current camera for the scene
    Camera *camera;

    /// All the drawable root nodes to render in the scene
    std::vector<HierarchicalNode *> rootNodes;

    /// The projection matrix for the scene
    glm::mat4 projection;

};


#endif //INC_3D_DEMOS_SCENE_H
