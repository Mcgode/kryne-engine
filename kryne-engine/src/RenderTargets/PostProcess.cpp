//
// Created by max on 25/07/2019.
//

#include <kryne-engine/Core/GeometricShapes.h>
#include "kryne-engine/RenderTargets/PostProcess.h"

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


PostProcess::PostProcess(Shader *shader, uint32_t width, uint32_t height, GLenum channels, GLenum type):
            FinalPostProcess(shader)
{
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
}


GLuint PostProcess::getTexture() const
{
    return texture;
}


void PostProcess::draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    FinalPostProcess::draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


PostProcess::~PostProcess()
{
    glDeleteFramebuffers(1, &this->fbo);
    glDeleteTextures(1, &this->texture);
}
