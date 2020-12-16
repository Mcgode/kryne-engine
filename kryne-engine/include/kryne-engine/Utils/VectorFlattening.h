/**
 * @file
 * @author Max Godefroy
 * @date 22/11/2020
 */

#ifndef INC_KRYNE_ENGINE_VECTORFLATTENING_H
#define INC_KRYNE_ENGINE_VECTORFLATTENING_H

#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


using namespace glm;


std::vector<float> flattenVector2(const std::vector<vec2>& data);


std::vector<float> flattenVector3(const std::vector<vec3>& data);


std::vector<float> flattenVector4(const std::vector<vec4>& data);

#endif //INC_KRYNE_ENGINE_VECTORFLATTENING_H
