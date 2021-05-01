/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#include "kryne-engine/Rendering/Additional/ShaderPass.h"

ShaderPass::ShaderPass(const string &name, const shared_ptr<Material> &material,
                       PostProcessPass::Priority priority)
        : PostProcessPass(priority, name), material(material)
{}


void ShaderPass::processPass(LoopRenderer *renderer, Framebuffer *readBuffer, Framebuffer *writeBuffer)
{
    this->material->setUniform("tDiffuse", readBuffer->retrieveColor(0));

    renderer->setTargetFramebuffer(writeBuffer);
    renderer->clearBuffer(true, true, true);
    renderer->textureRender(this->material.get());
}
