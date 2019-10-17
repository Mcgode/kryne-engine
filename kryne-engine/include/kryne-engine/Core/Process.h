//
// Created by max on 16/10/2019.
//

#ifndef INC_3D_DEMOS_PROCESS_H
#define INC_3D_DEMOS_PROCESS_H


#include <kryne-engine/Camera/Camera.h>
#include <kryne-engine/Rendering/Scene.h>
#include "Window.h"


/**
 * Handles the engine running process
 */
class Process {

public:

    /**
     * Handles the engine running process
     * @param camera        The base camera for the process
     * @param windowWidth   The window width for the rendering
     * @param windowHeight  The window height for the rendering
     */
    explicit Process(Camera *camera, uint16_t windowWidth = 1280, uint16_t windowHeight = 720);


    /**
     * Runs the render loop until the window is asked to be closed
     * @param parameters The additional parameters to pass down to the loop
     */
    void runProcess(AdditionalParameters *parameters = nullptr);


    /**
     * Retrieves the current window for the process.
     */
    [[nodiscard]] Window *getProcessWindow() const;


    /**
     * Retrieves the scene for the current engine process.
     */
    [[nodiscard]] Scene *getScene() const;


private:

    /// The render window for the current engine process.
    Window *processWindow;

    /// The scene for the current engine process.
    Scene *scene;

};


#endif //INC_3D_DEMOS_PROCESS_H