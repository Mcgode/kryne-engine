/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#include "kryne-engine/Textures/Texture.h"

Texture::Texture(GLuint id, GLenum textureType) :
        id(id),
        textureType(textureType),
        filterParams(GL_LINEAR)
{
    if (id)
        this->setFiltering(GL_LINEAR, GL_LINEAR);
}


Texture::~Texture()
{
    glDeleteTextures(1, &this->id);
}

void Texture::setFiltering(GLint minFilter, GLint magFilter)
{
    this->bindTexture();
    glTexParameteri(this->textureType, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(this->textureType, GL_TEXTURE_MAG_FILTER, magFilter);

    if (minFilter >= GL_NEAREST_MIPMAP_NEAREST && !this->mipMapped) {
        glGenerateMipmap(this->textureType);
        this->mipMapped = true;
    }

    this->filterParams = ivec2(minFilter, magFilter);
}
