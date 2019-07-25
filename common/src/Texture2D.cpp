//
// Created by max on 22/05/19.
//

#include <common/Texture2D.h>


namespace fs = boost::filesystem;


Texture2D::Texture2D(const std::string& filename, bool generateMipmap)
{
    void *data = stbi_load(filename.c_str(), &width, &height, &nbChannels, 0);

    if (data == nullptr) {
        std::cerr << "Failed to load texture file '" << filename << "'" << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format;
    switch (nbChannels) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Unsupported number of color channels: " << nbChannels << std::endl;
            exit(EXIT_FAILURE);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    if (generateMipmap)
        glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}


void Texture2D::textureSet(int textureID, Shader *shader, GLuint textureIndex, const std::string &uniformName)
{
    assert(textureIndex < 32);
    if (uniformName.length()) {
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        shader->setInt(uniformName, textureIndex);
    }

    glBindTexture(GL_TEXTURE_2D, textureID);

    if (uniformName.length())
        glActiveTexture(GL_TEXTURE0);
}


void Texture2D::setTexture(Shader *shader, GLuint textureIndex, const std::string &uniformName)
{
    Texture2D::textureSet(textureId, shader, textureIndex, uniformName);
}


Texture2D *Texture2D::getTexture(const std::string &directoryName, const std::string &filenameSuffix)
{
    fs::path dirPath("Textures/" + directoryName);

    if (!fs::exists(dirPath)) {
        std::cerr << "No such directory: '" << dirPath.string() << "'" << std::endl;
        exit(EXIT_FAILURE);
    }

    fs::directory_iterator end_itr;
    for (fs::directory_iterator itr(dirPath); itr != end_itr; ++itr)
    {
        fs::path p = itr->path();
        if (fs::is_regular_file(p) && p.string().find("_" + filenameSuffix + ".") != std::string::npos) {
            return new Texture2D(p.string());
        }
    }

    std::cerr << "No file found in directory with suffix: '" << filenameSuffix << "'" << std::endl;
    exit(EXIT_FAILURE);
}
