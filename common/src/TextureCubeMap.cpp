//
// Created by max on 02/08/2019.
//

#include "common/TextureCubeMap.h"

TextureCubeMap::TextureCubeMap(const std::string &folderName, const std::string &fileExtension)
{
    glGenTextures(1, &this->textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId);

    std::vector<std::string> faces
    {
        folderName + "/right" + fileExtension,
        folderName + "/left" + fileExtension,
        folderName + "/top" + fileExtension,
        folderName + "/bottom" + fileExtension,
        folderName + "/front" + fileExtension,
        folderName + "/back" + fileExtension
    };

    unsigned char *data;
    for (uint64_t i = 0; i < faces.size(); i++) {
        data = stbi_load(faces[i].c_str(), &width, &height, &nbChannels, 0);
        if (data == nullptr) {
            std::cerr << "Failed to load texture file '" << faces[i] << "'" << std::endl;
            exit(EXIT_FAILURE);
        }
        GLenum format = nbChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


void TextureCubeMap::setTexture(Shader *shader, GLuint textureIndex, const std::string &uniformName)
{
    Texture2D::textureSet(textureId, shader, textureIndex, uniformName, GL_TEXTURE_CUBE_MAP);
}
