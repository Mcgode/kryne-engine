/**
 * @file
 * @author max
 * @date 5/16/21.
 */

#ifdef KRYNE_ENGINE_VULKAN


#include "VulkanRenderer.h"


unordered_set<Entity *> VulkanRenderer::parseScene(Scene *scene)
{
    return std::unordered_set<Entity *>();
}


void VulkanRenderer::renderScene(Scene *scene)
{

}


void VulkanRenderer::renderMesh(RenderMesh *renderMesh, Camera *camera, Material *overrideMaterial)
{

}


void VulkanRenderer::handlePostProcessing()
{

}


void VulkanRenderer::textureRender(Material *material)
{

}


void VulkanRenderer::quadRender(Material *material, const ivec2 &start, const ivec2 &size)
{

}


void VulkanRenderer::setTargetFramebuffer(Framebuffer *framebuffer)
{

}


void VulkanRenderer::clearBuffer(bool color, bool depth, bool stencil)
{

}


void VulkanRenderer::renderCubeTexture(Framebuffer *framebuffer,
                                       Material *material,
                                       CubeTexture *cubeMap,
                                       int mipLevel)
{

}


#endif