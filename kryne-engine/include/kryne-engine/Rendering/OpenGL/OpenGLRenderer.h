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


class OpenGLContext;


class OpenGLRenderer: public LoopRenderer {

public:

    explicit OpenGLRenderer(OpenGLContext *context);

    void prepareFrame() override;

    void handleMesh(RenderMesh *renderMesh) override;

    vector<Entity *> parseScene(Scene *scene) override;

    void renderToScreen() override;

    void textureRender(Material *material) override;

protected:

    OpenGLContext *context;

    unique_ptr<PlaneBufferGeometry> fullscreenPlane;

};


#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>


