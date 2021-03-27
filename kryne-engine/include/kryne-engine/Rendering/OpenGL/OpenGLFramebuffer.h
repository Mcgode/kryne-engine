/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#pragma once


#include <kryne-engine/Rendering/Framebuffer.hpp>
#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>
#include <kryne-engine/Textures/Texture2D.h>


class OpenGLFramebuffer: public virtual Framebuffer {


public:

    OpenGLFramebuffer(uint16_t width, uint16_t height) : Framebuffer(width, height) {}

    shared_ptr<Texture> retrieveColor(uint8_t index) override;

    void setAsRenderTarget() override;

protected:

    GLuint fbo = 0;


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


