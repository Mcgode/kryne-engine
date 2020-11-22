//
// Created by Max Godefroy on 22/11/2020.
//

#include "kryne-engine/Utils/VectorFlattening.h"

std::vector<float> flattenVector2(const std::vector<vec2>& data)
{
    std::vector<float> flattened;
    for (auto v : data) {
        flattened.push_back(v.x);
        flattened.push_back(v.y);
    }
    return flattened;
}


std::vector<float> flattenVector3(const std::vector<vec3>& data)
{
    std::vector<float> flattened;
    for (auto v : data) {
        flattened.push_back(v.x);
        flattened.push_back(v.y);
        flattened.push_back(v.z);
    }
    return flattened;
}


std::vector<float> flattenVector4(const std::vector<vec4>& data)
{
    std::vector<float> flattened;
    for (auto v : data) {
        flattened.push_back(v.x);
        flattened.push_back(v.y);
        flattened.push_back(v.z);
        flattened.push_back(v.w);
    }
    return flattened;
}