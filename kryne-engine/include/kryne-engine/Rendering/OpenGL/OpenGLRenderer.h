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

    void prepareFrame() override;

    void handleMesh(RenderMesh *renderMesh) override;

    std::unordered_set<Entity *> parseScene(Scene *scene) override;

    void handlePostProcessing() override;

    void textureRender(Material *material) override;

protected:

    RenderingState *renderingState;

    unique_ptr<PlaneBufferGeometry> fullscreenPlane;

};


