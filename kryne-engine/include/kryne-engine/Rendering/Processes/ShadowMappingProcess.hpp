/**
 * @file
 * @author Max Godefroy
 * @date 17/04/2021.
 */

#pragma once


#include <kryne-engine/Rendering/RenderingProcess.hpp>
#include <kryne-engine/Systems/LightingRegistrySystem.hpp>


/**
 * @brief A rendering process that computes the shadow maps for lights.
 */
class ShadowMappingProcess : public RenderingProcess {

public:

    /**
     * @brief Initializes and couples the process.
     *
     * @param system    The lighting registry system to bind to.
     */
    ShadowMappingProcess(LightingRegistrySystem *system) : RenderingProcess("Shadow mapping"), system(system) {}

    // Override
    vector<Camera *> prepareFrame(LoopRenderer *renderer, Scene *scene) override;

    // Override
    void render(LoopRenderer *renderer,
                const std::vector<RenderMesh *> &meshes,
                const std::unordered_map<Camera *, LoopRenderer::FrustumCullingData> &frustumCullingMap) override;

    virtual ~ShadowMappingProcess();

protected:

    /// The lighting system this process is coupled with
    LightingRegistrySystem *system;


};


