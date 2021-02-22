/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once


#include "IRenderMesh.hpp"
#include <kryne-engine/Camera/ICamera.hpp>


class Scene;


/**
 * A renderer that will be run in the game loop.
 */
class LoopRenderer {

public:

    virtual void handleMesh(IRenderMesh *renderMesh) = 0;

    virtual void parseScene(Scene *scene) = 0;

    virtual void setCamera(weak_ptr<ICamera> camera) = 0;

};


#include <kryne-engine/3DObjects/Scene.h>