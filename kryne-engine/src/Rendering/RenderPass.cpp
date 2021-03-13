/**
 * @file
 * @author Max Godefroy
 * @date 06/03/2021.
 */

#include "kryne-engine/Rendering/RenderPass.h"

RenderPass::RenderPass(RenderPass::Order order, string name, bool customCull, bool renderGeometry) :
    order(order),
    name(move(name)),
    customCull(customCull),
    renderGeometry(renderGeometry)
{}
