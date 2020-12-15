/**
 * @file
 * @author Max Godefroy
 * @date 20/11/2020
 */

#ifndef INC_3D_DEMOS_RENDERER_H
#define INC_3D_DEMOS_RENDERER_H

#include <memory>
#include <kryne-engine/Core/Window.h>
#include <kryne-engine/3DObjects/Scene.h>
#include <kryne-engine/3DObjects/Mesh.h>
#include <kryne-engine/Camera/Camera.h>
#include <kryne-engine/Input/PlayerInput.h>
#include "RenderingState.hpp"

using namespace std;


/**
 * Handles an OpenGL window renderer
 */
class Renderer {

public:

    /**
     * Initializes a renderer window with a specific size.
     * @param width     The render window width in pixels.
     * @param height    The render window height in pixels.
     */
    Renderer(uint16_t width, uint16_t height);

    /**
     * Renders the provided scene.
     * @param scene     The scene to render.
     * @param camera    The camera used for rendering.
     */
    void render(Scene *scene, Camera *camera);

    /**
     * Retrieves the GLFW window for this renderer.
     */
    [[nodiscard]] inline const Window *getWindow() {
        return Renderer::associatedWindow.get();
    }

    /**
     * Returns the player input for this rendering window.
     */
#ifdef KRYNE_ENGINE_RENDERER_GET_PLAYER_INPUT_RAW
    [[nodiscard]] inline PlayerInput *getPlayerInput() const {
        return this->playerInput.get();
    }
#else
    [[nodiscard]] inline weak_ptr<PlayerInput> getPlayerInput() const {
        return weak_ptr<PlayerInput>(this->playerInput);
    }
#endif


protected:

    /**
     * Renders the provided object
     * @param object    The object to render.
     * @param camera    The camera used for rendering.
     */
    void renderObject(Object3D *object, Camera *camera);

    /**
     * Updates a specific object
     * @param object    The object to update.
     */
    static void updateObjects(Object3D *object);


protected:

    /// The GLFW window associated with this renderer.
    unique_ptr<Window> associatedWindow;

    /// The player input for this rendering window.
    shared_ptr<PlayerInput> playerInput;

    /// The current rendering state
    RenderingState renderingState;

};


#endif //INC_3D_DEMOS_RENDERER_H
