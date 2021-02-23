/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once




class RenderMesh;
class Camera;
class Scene;


/**
 * A renderer that will be run in the game loop.
 */
class LoopRenderer {

public:

    virtual void prepareFrame() = 0;

    virtual void handleMesh(RenderMesh *renderMesh) = 0;

    virtual void parseScene(Scene *scene) = 0;

    virtual void setCamera(Camera *camera) = 0;

};


#include <kryne-engine/3DObjects/Scene.h>
#include <kryne-engine/Rendering/RenderMesh.h>