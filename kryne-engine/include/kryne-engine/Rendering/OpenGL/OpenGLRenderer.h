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

    explicit OpenGLRenderer(RenderingState *renderingState, const ivec2 &size);

    // Override
    void prepareFrame() override;

    // Override
    void handleMesh(RenderMesh *renderMesh) override;

    // Override
    std::unordered_set<Entity *> parseScene(Scene *scene) override;

    // Override
    void finishSceneRendering(Scene *scene) override;

    // Override
    void handlePostProcessing() override;

    // Override
    void textureRender(Material *material) override;

    // Override
    void renderCubeTexture(Material *material, CubeTexture *cubeMap, const ivec2 &mapSize) override;

protected:

    /// The current OpenGL rendering state.
    RenderingState *renderingState;

    /// The fullscreen plane use for rendering fullscreen textures.
    unique_ptr<PlaneBufferGeometry> fullscreenPlane;

};


