/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#pragma once


#include <kryne-engine/Input/PlayerInput.h>
#include <kryne-engine/Rendering/LoopRenderer.h>


/**
 * The graphic context for the process.
 */
class GraphicContext {

public:

    /**
     * Asks the graphic context if the process should stop.
     */
    virtual bool shouldStop() = 0;

    /**
     * Asks the context to prepare for stopping.
     */
    virtual void stop() = 0;

    /**
     * Context code you need to run at the end of a loop.
     */
    virtual void endFrame() = 0;

    /**
     * Retrieves the player input associated with this context.
     */
    virtual PlayerInput *getPlayerInput() = 0;

    /**
     * Retrieves the renderer for this context.
     */
    virtual LoopRenderer *getRenderer() = 0;

    /**
     * @brief Instantiates a framebuffer for this context
     */
    virtual unique_ptr<Framebuffer> makeFramebuffer(const ivec2 &size) = 0;

};
