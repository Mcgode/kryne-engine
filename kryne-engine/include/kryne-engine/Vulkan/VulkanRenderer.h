/**
 * @file
 * @author max
 * @date 5/16/21.
 */

#pragma once

#ifdef KRYNE_ENGINE_VULKAN


#include "../Rendering/LoopRenderer.h"


class VulkanRenderer: public LoopRenderer
{
public:

    // Override
    unordered_set<Entity *> parseScene(Scene *scene) override;

    // Override
    void renderScene(Scene *scene) override;

    //Override
    void renderMesh(RenderMesh *renderMesh, Camera *camera, Material *overrideMaterial) override;

    //Override
    void handlePostProcessing() override;

    //Override
    void textureRender(Material *material) override;

    //Override
    void quadRender(Material *material, const ivec2 &start, const ivec2 &size) override;

    //Override
    void setTargetFramebuffer(Framebuffer *framebuffer) override;

    //Override
    void clearBuffer(bool color, bool depth, bool stencil) override;

    //Override
    void renderCubeTexture(Framebuffer *framebuffer, Material *material, CubeTexture *cubeMap, int mipLevel) override;
};


#endif


