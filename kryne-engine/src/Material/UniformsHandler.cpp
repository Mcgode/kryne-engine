//
// Created by Max Godefroy on 24/11/2020.
//

#include "kryne-engine/Material/UniformsHandler.h"


void UniformsHandler::setUniform(const string &name, const UniformTypes &value)
{
    auto l = this->uniforms.find(name);
    if (l != this->uniforms.end()) {
        l->second.first = value;
    } else {
        GLint location = glGetUniformLocation(this->programId, name.c_str());
        this->uniforms.emplace(name, make_pair(value, location));
    }
}


void UniformsHandler::notifyUniformLocationsNeedUpdate()
{
    for (auto &entry : this->uniforms)
        entry.second.second = -2;
}


bool UniformsHandler::removeUniform(const string &name)
{
    return this->uniforms.erase(name) > 0;
}


void UniformsHandler::updateUniforms()
{
    static UniformsSetter setter;
    for (auto &entry : this->uniforms) {
        const auto data = entry.second;

        if (data.second < -1) {
            entry.second.second = glGetUniformLocation(this->programId, entry.first.c_str());
        }

        if (data.second > -1) {
            UniformLocationType location = data.second;
            boost::apply_visitor(setter, data.first, location);
        }
    }
}
