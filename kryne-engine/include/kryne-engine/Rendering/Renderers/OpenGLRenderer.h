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

    void handleMesh(RenderMesh *renderMesh) override;

    void parseScene(Scene *scene) override {};

    void setCamera(Camera *camera) override { this->mainCamera = camera; }

protected:

    Camera *mainCamera {};

    OpenGLContext *context;

};


#include <kryne-engine/Core/GraphicContext/OpenGLContext.h>


