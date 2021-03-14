/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#include "kryne-engine/Rendering/PostProcessPass.h"


PostProcessPass::PostProcessPass(PostProcessPass::Priority priority, string name) : priority(priority),
                                                                                    name(move(name)) {}
