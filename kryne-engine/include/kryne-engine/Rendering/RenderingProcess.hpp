/**
 * @file
 * @author Max Godefroy
 * @date 17/04/2021.
 */

#pragma once


#include <string>
#include <utility>

#include <kryne-engine/Core/Process.h>
#include "LoopRenderer.h"


class RenderingProcess {

public:

    /**
     * @brief Method called at the start of the frame. Use it to parse the scene.
     *
     * @param renderer  The process renderer.
     * @param scene     The scene that will be rendered.
     *
     * @returns A list of cameras that will be used for computing frustum culling. Can be empty.
     */
    virtual vector<Camera *> prepareFrame(const LoopRenderer *renderer, Scene *scene) = 0;

    /**
     * @breif Method called to run this process
     *
     * @param renderer          The process renderer.
     * @param meshes            The list of render meshes for the scene.
     * @param frustumCullingMap The frustum culling map.
     */
    virtual void render(LoopRenderer *renderer, const std::vector<RenderMesh *> &meshes,
                        const std::unordered_map<Camera *, LoopRenderer::FrustumCullingData> &frustumCullingMap) = 0;

    /// @brief Retrieves the process name
    [[nodiscard]] const string &getName() const { return name; }

    /// @brief Retrieves whether the process is enabled
    [[nodiscard]] bool isEnabled() const { return enabled; }

    /// @brief Updates whether the process is enabled
    void setEnabled(bool val) { RenderingProcess::enabled = val; }

protected:

    /**
     * @brief Protected constructor, for children to call.
     *
     * @param name The name of the process
     */
    explicit RenderingProcess(std::string name) : name(std::move(name)) {}

protected:

    /// The name of the process
    std::string name;

    /// Whether this process is enabled and should run
    bool enabled = true;

};


