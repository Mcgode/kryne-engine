//
// Created by max on 02/08/2019.
//

#include "kryne-engine/Textures/CubeTexture.h"

CubeTexture::CubeTexture(const vector<string> &filenames) :
    Texture(0, GL_TEXTURE_CUBE_MAP)
{
    int32_t width, height, nbChannels;
    int32_t pw, ph, pnc;
    glGenTextures(1, &id);
    this->setFiltering(GL_LINEAR, GL_LINEAR);

    uint8_t *data;
    for (uint64_t i = 0; i < filenames.size(); i++)
    {
        data = stbi_load(filenames[i].c_str(), &width, &height, &nbChannels, 0);

        if (data == nullptr)
        {
            cerr << "Failed to load texture file '" << filenames[i] << "'" << endl;
            throw EXIT_FAILURE;
        }

        if (i > 0 && (pw != width || ph != height || nbChannels != pnc))
        {
            cerr << "Incoherent values between images" << endl;
            throw EXIT_FAILURE;
        }

        GLenum format = nbChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
        );

        pw = width;
        ph = height;
        pnc = nbChannels;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


shared_ptr<CubeTexture> CubeTexture::loadFilesSync(const vector<string> &filenames)
{
    using namespace boost::filesystem;

    for (const auto &f: filenames)
    {
        const auto p = path(f);
        if (!exists(p) || !is_regular_file(p))
        {
            cerr << "No file named '" << f << "'" << endl;
            throw EXIT_FAILURE;
        }
    }

    return shared_ptr<CubeTexture>(new CubeTexture(filenames));
}
