/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <kryne-engine/Rendering/RenderingState.hpp>
#include "GraphicContext.h"


class OpenGLRenderer;


using namespace std;

/**
 * An OpenGL graphic context for the process
 */
class OpenGLContext: public GraphicContext {

friend class OpenGLRenderer;

public:

    /**
     * Tries to initialize the OpenGL context.
     * @param baseWidth     The main window width. Defaults to 1280.
     * @param baseHeight    The main window height. Defaults to 720.
     * @param majorVersion  The OpenGL major version. Defaults to 3.
     * @param minorVersion  The OpenGL minor version. Defaults to 3.
     * @param profile       The OpenGL profile. Defaults to core profile.
     */
    explicit OpenGLContext(GLuint baseWidth = 1280, GLuint baseHeight = 720,
                           GLint majorVersion = 4, GLint minorVersion = 5,
                           GLint profile = GLFW_OPENGL_CORE_PROFILE);

    /**
     * @copydoc GraphicContext::shouldStop()
     */
    bool shouldStop() override;

    /**
     * @copydoc GraphicContext::stop()
     */
    void stop() override;

    /**
     * @copydoc GraphicContext::endFrame()
     */
    void endFrame() override;

    /**
     * @copydoc GraphicContext::getPlayerInput()
     */
    PlayerInput *getPlayerInput() override;

    LoopRenderer *getRenderer() override;

    GLFWwindow *getWindow() const { return this->mainWindow; }

    /**
     * Terminates the OpenGL context.
     */
    virtual ~OpenGLContext();

protected:

    /// The OpenGL main window.
    GLFWwindow *mainWindow;

    /// The PlayerInput for this context.
    shared_ptr<PlayerInput> input;

    /// The rendering state of the graphic context.
    unique_ptr<RenderingState> renderingState {};

    /// The renderer for this context
    unique_ptr<OpenGLRenderer> renderer {};

};


#include <kryne-engine/Rendering/OpenGL/OpenGLRenderer.h>
