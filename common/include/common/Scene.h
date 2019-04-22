//
// Created by max on 22/04/19.
//

#ifndef INC_3D_DEMOS_SCENE_H
#define INC_3D_DEMOS_SCENE_H


#include "Camera.h"
#include "Window.h"
#include "HierarchicalNode.h"

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
    void updateCamera(Camera *newCamera);

    /**
     * Adds a new drawable root node to the scene
     * @param node The node to add to the scene
     */
    void addDrawable(HierarchicalNode *node);


    void mainRenderLoop(std::map<std::string, void *> *params);

    ~Scene();

private:

    /// The window where the rendering display will happen
    Window *window;

    /// The current camera for the scene
    Camera *camera;

    /// All the drawable root nodes to render in the scene
    std::vector<HierarchicalNode *> rootNodes;

};


#endif //INC_3D_DEMOS_SCENE_H
