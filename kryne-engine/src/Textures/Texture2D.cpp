//
// Created by max on 22/05/19.
//

#include <kryne-engine/Textures/Texture2D.h>


namespace fs = boost::filesystem;


Texture2D::Texture2D(const std::string& filename, bool generateMipmap): Texture2D(0)
{
    int32_t width, height, nbChannels;
    void *data = stbi_load(filename.c_str(), &width, &height, &nbChannels, 0);

    if (data == nullptr) {
        std::cerr << "Failed to load texture file '" << filename << "'" << std::endl;
        throw EXIT_FAILURE;
    }

    glGenTextures(1, &id);
    this->setFiltering(GL_LINEAR, GL_LINEAR);

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
            throw EXIT_FAILURE;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

shared_ptr<Texture2D> Texture2D::loadFromFileSync(const string &filename)
{
    const auto path = fs::path(filename);
    if (fs::exists(path) && fs::is_regular_file(path))
        return shared_ptr<Texture2D>(new Texture2D(path.string()));

    cerr << "Unable to find file '" << filename << "'" << endl;
    throw EXIT_FAILURE;
}
