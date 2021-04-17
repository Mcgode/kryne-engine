/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once


#include <kryne-engine/Rendering/LoopRenderer.h>
#include <kryne-engine/Geometry/PlaneBufferGeometry.h>

#include "OpenGLFramebuffer.h"
#include "OpenGLScreenFramebuffer.hpp"


class OpenGLRenderer: public LoopRenderer {

public:

    explicit OpenGLRenderer(GraphicContext *context, RenderingState *renderingState, const ivec2 &size);

    /// @brief Resets special states that are usually disabled
    void defaultStateReset();

    // Override
    void prepareFrame() override;

    // Override
    void handleMesh(RenderMesh *renderMesh) override;

    // Override
    std::unordered_set<Entity *> parseScene(Scene *scene) override;

    // Override
    void renderScene(Scene *scene) override;

    // Override
    void handlePostProcessing() override;

    // Override
    void textureRender(Material *material) override;

    // Override
    void renderCubeTexture(Framebuffer *framebuffer, Material *material, CubeTexture *cubeMap, int mipLevel) override;

    //Override
    void quadRender(Material *material, const ivec2 &start, const ivec2 &size) override;

protected:

    /// The current OpenGL rendering state.
    RenderingState *renderingState;

    /// The fullscreen plane use for rendering fullscreen textures.
    unique_ptr<PlaneBufferGeometry> fullscreenPlane;

};


