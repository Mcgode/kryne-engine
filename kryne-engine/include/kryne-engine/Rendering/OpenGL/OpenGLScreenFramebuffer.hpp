/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once

#include <kryne-engine/Rendering/Framebuffer.hpp>


class OpenGLScreenFramebuffer : public Framebuffer {

public:

    OpenGLScreenFramebuffer(RenderingState *state, uint16_t width, uint16_t height) : Framebuffer(width, height), state(state) {}

    // Override
    void addColorAttachment() override {}

    // Override
    void setUpDepthLayer() override {}

    // Override
    shared_ptr<Texture> retrieveColor(uint8_t index) override { return nullptr; }

protected:

    // Override
    void setAsRenderTarget() override
    {
        assertIsMainThread();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        state->setCurrentFramebuffer(this);
    }

    // Override
    void sizeUpdated() override {}

protected:

    RenderingState *state;

};