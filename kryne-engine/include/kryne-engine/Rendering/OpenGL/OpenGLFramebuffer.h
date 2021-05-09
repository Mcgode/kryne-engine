/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#pragma once


#ifdef KRYNE_ENGINE_OPEN_GL


#include <kryne-engine/Rendering/Framebuffer.hpp>
#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>
#include <kryne-engine/Textures/Texture2D.h>


class OpenGLFramebuffer: public virtual Framebuffer {


public:

    OpenGLFramebuffer(RenderingState *state, uint16_t width, uint16_t height) : Framebuffer(width, height), state(state) {}

    // Override
    shared_ptr<Texture> retrieveColor(uint8_t index) override;

    shared_ptr<Texture> retrieveDepth() override;

protected:

    // Override
    void setAsRenderTarget() override;

    // Override
    void sizeUpdated() override;

protected:

    /// The FBO index
    GLuint fbo = 0;

    /// The OpenGL state tracker
    RenderingState *state;


public:

    void addColorAttachment() override;

    void setUpDepthLayer() override;

protected:

    struct AttachmentData {

        shared_ptr<Texture> texture {};

    };

    vector<AttachmentData> attachments;

    unique_ptr<AttachmentData> depthStencilAttachment {};

};


#endif

