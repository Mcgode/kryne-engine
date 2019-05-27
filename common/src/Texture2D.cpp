//
// Created by max on 22/05/19.
//

#include "common/Texture2D.h"

Texture2D::Texture2D(const std::string& filename, bool generateMipmap)
{
    void *data = stbi_load(filename.c_str(), &width, &height, &nbChannels, 0);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    if (generateMipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}


void Texture2D::textureSet(int textureID, Shader *shader, GLuint textureIndex, const std::string *uniformName)
{
    assert(textureIndex < 32);
    if (uniformName != nullptr) {
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        shader->setInt(*uniformName, textureIndex);
    }

    glBindTexture(GL_TEXTURE_2D, textureID);

    if (uniformName != nullptr)
        glActiveTexture(GL_TEXTURE0);
}


void Texture2D::setTexture(Shader *shader, GLuint textureIndex, std::string *uniformName)
{
    Texture2D::textureSet(textureId, shader, textureIndex, uniformName);
}
