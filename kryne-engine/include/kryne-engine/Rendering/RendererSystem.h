/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#pragma once


#include <kryne-engine/Core/System.h>
#include "RenderMesh.h"


class RendererSystem: public System {

public:

    /// Initialize the renderer system
    explicit RendererSystem(Process *process): System(process) {}

    /// @copydoc System::runSystem()
    void runSystem(Scene *scene, bool allowConcurrentChunks) override;

    virtual void renderObject(RenderMesh *renderMesh) = 0;

};



