/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once


#include <vector>

#include <kryne-engine/enums/RenderMode.h>


class RenderMesh;
class Camera;
class Scene;
class Entity;


/**
 * A renderer that will be run in the game loop.
 */
class LoopRenderer {

public:

    virtual void prepareFrame() = 0;

    virtual void handleMesh(RenderMesh *renderMesh) = 0;

    virtual std::vector<Entity *> parseScene(Scene *scene) = 0;

    virtual void setCamera(Camera *camera) = 0;

    [[nodiscard]] RenderMode getRenderingMode() const { return this->renderMode; }

    virtual void setRenderingMode(RenderMode mode) { this->renderMode = mode; }

protected:

    RenderMode renderMode = ForwardRendering;

};


#include <kryne-engine/Core/Entity.h>
#include <kryne-engine/3DObjects/Scene.h>
#include <kryne-engine/Rendering/RenderMesh.h>