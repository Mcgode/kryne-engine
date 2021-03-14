/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once


#include <kryne-engine/Rendering/LoopRenderer.h>


class OpenGLContext;


class OpenGLRenderer: public LoopRenderer {

public:

    explicit OpenGLRenderer(OpenGLContext *context) : context(context) {};

    void prepareFrame() override;

    void handleMesh(RenderMesh *renderMesh) override;

    vector<Entity *> parseScene(Scene *scene) override;

protected:

    OpenGLContext *context;

};


#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>


