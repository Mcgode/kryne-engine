//
// Created by max on 25/07/2019.
//

#include <common/GeometricShapes.h>
#include "common/RenderTargets/PostProcess.h"

GLenum getInternalColorFormat(GLenum channels, GLenum type)
{
    switch (type) {
        case GL_FLOAT:
            switch (channels) {
                case GL_RGB:
                    return GL_RGB32F;
                case GL_RGBA:
                    return GL_RGBA32F;
                default:
                    cerr << "Unsupported channels value for type GL_FLOAT: " << channels << endl;
                    exit(EXIT_FAILURE);
            }
        case GL_UNSIGNED_BYTE:
            return channels;
        default:
            cerr << "Unsupported type: " << type << endl;
            exit(EXIT_FAILURE);
    }
}


PostProcess::PostProcess(Shader *shader, uint32_t width, uint32_t height, GLenum channels, GLenum type) {
    this->postProcessShader = shader;

    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    GLenum colorFormat = getInternalColorFormat(channels, type);

    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, channels, type, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

    vector<glm::vec3> vertices;
    vector<glm::vec2> textureCoordinates;
    generateQuadShape(2.0, &vertices, nullptr, &textureCoordinates);
    this->va = new VertexArray(&vertices);
    this->va->add_array(&textureCoordinates);
}


void PostProcess::setIntUniform(int value, const string &uniformName)
{
    this->postProcessShader->setInt(uniformName, value);
}


void PostProcess::setFloatUniform(float value, const string &uniformName)
{
    this->postProcessShader->setFloat(uniformName, value);
}


void PostProcess::setVec2Uniform(glm::vec2 vector, const string &uniformName)
{
    this->postProcessShader->setVec2(uniformName, vector);
}


void PostProcess::setVec3Uniform(glm::vec3 vector, const string &uniformName)
{
    this->postProcessShader->setVec3(uniformName, vector);
}


void PostProcess::setVec4Uniform(glm::vec4 vector, const string &uniformName)
{
    this->postProcessShader->setVec4(uniformName, vector);
}


void PostProcess::setTextureUniform(Texture2D *textureUniform, const string& uniformName)
{
    auto position = this->textures.find(uniformName);
    if (position != this->textures.end()) {
        if (nextIndex >= 32) {
            cerr << "Maximum amount of individual textures used for this shader" << endl;
            exit(EXIT_FAILURE);
        }
        this->textures.insert(pair<string, uint8_t>(uniformName, this->nextIndex));
        textureUniform->setTexture(this->postProcessShader, this->nextIndex++, uniformName);
    } else {
        textureUniform->setTexture(this->postProcessShader, position->second, uniformName);
    }

}


GLuint PostProcess::getTexture() const
{
    return texture;
}


void PostProcess::draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    this->postProcessShader->use();
    this->va->execute(GL_TRIANGLES);
    Shader::resetUse();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
