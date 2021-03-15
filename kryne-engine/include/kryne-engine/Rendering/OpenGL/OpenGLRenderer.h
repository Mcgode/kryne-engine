/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once


#include <kryne-engine/Rendering/LoopRenderer.h>
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

protected:

    OpenGLContext *context;

};


#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>


