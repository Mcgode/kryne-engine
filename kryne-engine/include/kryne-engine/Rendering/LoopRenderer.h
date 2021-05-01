/**
 * @file
 * @author Max Godefroy
 * @date 06/02/2021.
 */

#pragma once


#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#include <kryne-engine/enums/RenderMode.h>
#include <kryne-engine/Math/Frustum.hpp>
#include <kryne-engine/Utils/UniquePtrVector.hpp>
#include "Framebuffer.hpp"


class GraphicContext;
class RenderMesh;
class Camera;
class Scene;
class Entity;
class PostProcessPass;
class Material;
class ShaderMaterial;
class BoxBufferGeometry;
class CubeTexture;
class PMREMGenerator;
class RenderingProcess;


using namespace std;


/**
 * @brief A renderer that will be run in the game loop.
 */
class LoopRenderer {

public:

    /// @brief Retrieves the current graphical context
    [[nodiscard]] GraphicContext *getContext() const { return this->context; }

    /**
     * @brief A method that will be called at the start of every frame.
     *
     * @details
     * Ideal for resetting internal data.
     */
    virtual void prepareFrame();


    void registerMesh(RenderMesh *mesh);

    /**
     * @brief Parses the scene to render in the search of essential data for this frame.
     *
     * @param scene     The current scene that is going to be rendered.
     *
     * @return A set containing all the meshes that need to have their game logic processed first
     */
    virtual std::unordered_set<Entity *> parseScene(Scene *scene) = 0;

    /**
     * @brief Sets the main camera for rendering the scene.
     *
     * @details
     * This camera will be used only on the next frame, to prevent camera change mid-frame.
     *
     * @param camera    The new main camera.
     */
    virtual void setCamera(Camera *camera) { this->mainCamera = camera; }

    /// @brief Retrieves the current main camera
    [[nodiscard]] Camera *getMainCamera() const { return this->mainCamera; }

    /**
     * @brief Retrieves the current rendering mode
     */
    [[nodiscard]] RenderMode getRenderingMode() const { return this->renderMode; }

    /**
     * @brief Changes the current rendering mode.
     */
    virtual void setRenderingMode(RenderMode mode) { this->renderMode = mode; }

    /**
     * @brief Finishes the scene rendering, in prevision of the post-processing.
     */
    virtual void renderScene(Scene *scene) = 0;

    /**
     * @brief Renders a mesh to the framebuffer.
     *
     * @param renderMesh        The mesh to render.
     * @param overrideMaterial  Pass this material to override the default mesh material. Set to nullptr to ignore
     *                          this feature.
     */
    virtual void renderMesh(RenderMesh *renderMesh, Camera *camera, Material *overrideMaterial) = 0;

    /**
     * @brief Handles the post processing and the rendering to screen.
     */
    virtual void handlePostProcessing() = 0;

    /**
     * @brief Renders a fullscreen texture.
     *
     * @details
     * Used in particular for postprocessing passes.
     *
     * @param material The material for rendering this texture
     */
    virtual void textureRender(Material *material) = 0;

    /***
     * @brief Renders a quad on a part of the screen
     *
     * @param material  The material for rendering this quad
     * @param start     The quad upper corner position
     * @param size      The quad size
     */
    virtual void quadRender(Material *material, const ivec2 &start, const ivec2 &size) = 0;

protected:

    /**
     * @brief Initializes the base renderer
     *
     * @param context               The graphical context for this renderer.
     * @param screenFramebuffer     The framebuffer corresponding to the displayed window.
     * @param readFramebuffer       The framebuffer color data will be read from.
     * @param writeFramebuffer      The framebuffer color data will be written to.
     * @param size                  The current window size, in pixels.
     */
    LoopRenderer(GraphicContext *context,
                 unique_ptr<Framebuffer> screenFramebuffer,
                 unique_ptr<Framebuffer> readFramebuffer,
                 unique_ptr<Framebuffer> writeFramebuffer,
                 const ivec2 &size);

protected:

    /// The graphical context of the renderer
    GraphicContext *context;

    /// The current rendering mode.
    RenderMode renderMode = ForwardRendering;

    /// The main camera for this frame.
    Camera *mainCamera {};

    /// The framebuffer corresponding to the displayed window.
    unique_ptr<Framebuffer> screenFramebuffer;

    /// The meshes to render this frame
    vector<RenderMesh *> meshesForFrame;

    /// A mutex for handling concurrent write of #meshesForFrame
    mutex meshesMutex;


// ================
// Post processing
// ================

public:

    /**
     * @brief Inserts a post process pass at the last valid position in the list.
     *
     * @param pass A unique pointer owning the pass to insert.
     */
    void addPass(unique_ptr<PostProcessPass> pass);

    /**
     * @brief Inserts a post process pass after another one.
     *
     * @details
     * The process will fail if it either can't find the other pass, or if the position is invalid in regards to
     * pass priority (defined by PostProcessPass::priority).
     *
     * @param pass The pass to insert.
     * @param name The name of the pass to insert after.
     * @return true if insertion was successful, false otherwise.
     */
    bool addPassAfter(unique_ptr<PostProcessPass> pass, const string &name);

    /**
     * @brief Inserts a post process pass before another one.
     *
     * @details
     * The process will fail if it either can't find the other pass, or if the position is invalid in regards to
     * pass priority (defined by PostProcessPass::priority).
     *
     * @param pass The pass to insert.
     * @param name The name of the pass to insert before.
     * @return true if insertion was successful, false otherwise.
     */
    bool addPassBefore(unique_ptr<PostProcessPass> pass, const string &name);

    /**
     * @brief Tries to remove a post process pass.
     *
     * @param name The name of the post process pass to remove.
     * @return A unique pointer owning the pass. Can be null if there was no pass with the provided name.
     */
    unique_ptr<PostProcessPass> removePass(const string &name);

    /**
     * @brief Updates the renderer size.
     *
     * @param size  The new size for the renderer.
     */
    void updateRendererSize(const ivec2 &size);

protected:

    /**
     * @brief Swaps the write and read framebuffers.
     */
    inline void swapBuffers() { swap(readFramebuffer, writeFramebuffer); }

protected:

    /// The ordered list of post process passes.
    Utils::UniquePtrVector<PostProcessPass> postProcessPasses {};

    /// The post process passes to use for this frame.
    vector<PostProcessPass *> framePostProcessPasses {};

    /// The framebuffer color data will be read from.
    unique_ptr<Framebuffer> readFramebuffer;

    /// The framebuffer color data will be written to.
    unique_ptr<Framebuffer> writeFramebuffer;

    /// The renderer size in pixels.
    ivec2 rendererSize;


// ==================
// Framebuffer
// ==================

public:

    /**
     * @brief Sets the provided framebuffer as the new render target.
     *
     * @param framebuffer The framebuffer to write to
     */
    virtual void setTargetFramebuffer(Framebuffer *framebuffer) = 0;

    /**
     * @brief Clears the current framebuffer
     *
     * @param color     Set to true to clear color
     * @param depth     Set to true to clear depth
     * @param stencil   Set to true to clear stencil
     */
    virtual void clearBuffer(bool color, bool depth, bool stencil) = 0;


// ==================
// Frustum culling
// ==================

protected:

    /**
     * @brief Computes the frustum culling of a given mesh for all the cameras.
     *
     * @param mesh  The mesh to test the frustum culling on.
     */
    void computeFrustumCulling(RenderMesh *mesh);

public:

    struct FrustumCullingData {

        Math::Frustum frustum;

        std::unordered_map<RenderMesh *, bool> culledMeshes {};

        explicit FrustumCullingData(Camera *camera);

    };

protected:

    /// The frustum culling state of meshes.
    std::unordered_map<Camera *, FrustumCullingData> frustumCulled;

    /// A mutex for handling concurrent frustum culling
    mutex frustumCullingMutex;


// ==================
// Cube rendering
// ==================

public:

    /**
     * @brief Renders the provided material to a cube.
     *
     * @param framebuffer   The framebuffer used for this rendering. It is assumed to be bound beforehand.
     * @param material      The material used for rendering the texture.
     * @param cubeTexture   The texture which is rendered to.
     * @param mipLevel      The mip map level for this texture.
     */
    virtual void renderCubeTexture(Framebuffer *framebuffer, Material *material, CubeTexture *cubeMap, int mipLevel) = 0;

protected:

    /// The cube geometry used for cube maps rendering.
    shared_ptr<BoxBufferGeometry> cubeGeometry;

    /// The mesh data for drawing a skybox
    shared_ptr<ShaderMaterial> skyboxMaterial;

    unique_ptr<PMREMGenerator> pmremGenerator;


// ==================
// Rendering processes
// ==================

public:

    /**
     * @brief Inserts a rendering process to be run before the final frame render.
     *
     * @param process   The process to insert
     */
    void addProcess(unique_ptr<RenderingProcess> process);

    /**
     * @brief Inserts a rendering process after another one.
     *
     * @details
     * The method will fail if it can't find the other process.
     *
     * @param process   The process to insert.
     * @param name      The name of the process to insert before.
     *
     * @return true if insertion was successful, false otherwise.
     */
    bool addProcessAfter(unique_ptr<RenderingProcess> process, const string &name);

    /**
     * @brief Inserts a rendering process before another one.
     *
     * @details
     * The method will fail if it can't find the other process.
     *
     * @param process   The process to insert.
     * @param name      The name of the process to insert before.
     *
     * @return true if insertion was successful, false otherwise.
     */
    bool addProcessBefore(unique_ptr<RenderingProcess> process, const string &name);

protected:

    Utils::UniquePtrVector<RenderingProcess> processes {};

};


#include "PostProcessPass.h"
#include "RenderingProcess.hpp"
#include <kryne-engine/Core/Entity.h>
#include <kryne-engine/Camera/Camera.h>
#include <kryne-engine/3DObjects/Scene.h>
#include <kryne-engine/Rendering/RenderMesh.h>
#include <kryne-engine/Geometry/BoxBufferGeometry.h>
#include <kryne-engine/Material/ShaderMaterial.hpp>
#include <kryne-engine/Rendering/Additional/PMREMGenerator.hpp>