/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once


#include <kryne-engine/Rendering/PostProcessPass.h>


class ShaderPass : public PostProcessPass {

public:

    ShaderPass(const string &name, const shared_ptr<Material> &material,
               PostProcessPass::Priority priority = VisualEffect);

    [[nodiscard]] const shared_ptr<Material> &getMaterial() const { return material; }

    void setMaterial(const shared_ptr<Material> &newMaterial) { ShaderPass::material = newMaterial; }

protected:

    shared_ptr<Material> material;


public:

    void processPass(LoopRenderer *renderer, Framebuffer *readBuffer, Framebuffer *writeBuffer) override;

};


