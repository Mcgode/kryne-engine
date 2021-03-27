/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#include "kryne-engine/Rendering/OpenGL/OpenGLFramebuffer.h"


void OpenGLFramebuffer::addColorAttachment()
{
    this->attachments.emplace_back();
}


shared_ptr<Texture> OpenGLFramebuffer::retrieveColor(uint8_t index)
{
    return this->attachments[index].texture;
}


void OpenGLFramebuffer::setAsRenderTarget()
{
    assertIsMainThread();

    if (this->fbo == 0)
        glGenFramebuffers(1, &this->fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    bool changed = false;

    for (size_t i = 0; i < this->attachments.size(); i++)
    {
        auto &attachment = this->attachments[i];

        if (attachment.texture == nullptr)
        {
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);

            attachment.texture = make_shared<Texture2D>(texture);
            changed = true;
        }
    }

    if (this->depthStencilAttachment)
    {
        if (this->depthStencilAttachment->texture == nullptr)
        {
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->width, this->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
            this->depthStencilAttachment->texture = make_shared<Texture2D>(texture);
            changed = true;
        }
    }

    if (changed)
    {
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
}


void OpenGLFramebuffer::setUpDepthLayer()
{
    this->depthStencilAttachment = make_unique<AttachmentData>();
}
