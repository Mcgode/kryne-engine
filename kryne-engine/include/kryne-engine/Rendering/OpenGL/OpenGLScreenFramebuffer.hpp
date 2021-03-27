/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once

#include <kryne-engine/Rendering/Framebuffer.hpp>


class OpenGLScreenFramebuffer : public Framebuffer {

public:

    OpenGLScreenFramebuffer(uint16_t width, uint16_t height) : Framebuffer(width, height) {}

    void setAsRenderTarget() override
    {
        assertIsMainThread();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void addColorAttachment() override {}

    void setUpDepthLayer() override {}

    shared_ptr<Texture> retrieveColor(uint8_t index) override { return nullptr; }

};