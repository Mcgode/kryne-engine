/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#pragma once


virtual class Framebuffer {

public:

    virtual void setAsRenderTarget() = 0;

    Texture *retrieveFramebufferTexture() = 0;

};