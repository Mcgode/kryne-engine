//
// Created by Max Godefroy on 24/11/2020.
//

#include "kryne-engine/Material/UniformsHandler.h"


void UniformsHandler::setUniform(const string &name, const UniformTypes &value)
{
    scoped_lock<mutex> lock(this->uniformsMutex);

    auto l = this->uniforms.find(name);
    if (l != this->uniforms.end()) {
        l->second.first = value;
    } else {
        GLint location = (*this->programId != 0) ? glGetUniformLocation(*this->programId, name.c_str()) : -2;
        this->uniforms.emplace(name, make_pair(value, location));
    }
}


void UniformsHandler::notifyUniformLocationsNeedUpdate()
{
    scoped_lock<mutex> l(this->uniformsMutex);

    for (auto &entry : this->uniforms)
        entry.second.second = -2;

    this->activeTextures.clear();
    this->nextTextureIndex = GL_TEXTURE0;
}


bool UniformsHandler::removeUniform(const string &name)
{
    scoped_lock<mutex> l(this->uniformsMutex);

    auto it = this->uniforms.find(name);

    if (it != this->uniforms.end())
    {
        if (it->second.second >= 0)
            this->activeTextures.erase(it->second.second);
        this->uniforms.erase(it);
        return true;
    }
    return false;
}


void UniformsHandler::updateUniforms()
{
    if (*this->programId == 0)
        return;

    scoped_lock<mutex> l(this->uniformsMutex);

    for (auto &entry : this->uniforms) {
        const auto data = entry.second;

        if (data.second < -1) {
            entry.second.second = glGetUniformLocation(*this->programId, entry.first.c_str());
        }

        if (data.second > -1) {
            UniformLocationType location = data.second;
            boost::apply_visitor(this->setter, data.first, location);
        }
    }
}


void UniformsHandler::setTexture(const shared_ptr<Texture> &texture, GLint location)
{
    if (location < 0) return;

    const auto it = this->activeTextures.find(location);
    GLint activeTexture;
    if (it != this->activeTextures.end())
    {
        activeTexture = it->second;
    }
    else
    {
        activeTexture = this->nextTextureIndex++;
        this->activeTextures.insert(make_pair(location, activeTexture));
    }

    glActiveTexture(activeTexture);
    texture->bindTexture();
    glUniform1i(location, activeTexture - GL_TEXTURE0);

}
