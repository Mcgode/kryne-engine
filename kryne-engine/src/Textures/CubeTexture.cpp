//
// Created by max on 02/08/2019.
//

#include "kryne-engine/Textures/CubeTexture.h"


shared_ptr<CubeTexture> CubeTexture::loadFiles(const vector<string> &filenames)
{
    using namespace boost::filesystem;

    auto texture = make_shared<CubeTexture>(0);

    const auto func = [texture, filenames]() {

        if (filenames.size() < 6)
            throw runtime_error("Not enough filenames were provided");

        for (const auto &f: filenames)
        {
            const auto p = path(f);
            if (!exists(p) || !is_regular_file(p))
                runtime_error("No file named '" + f + "'");
        }

        int32_t width[6], height[6], nbChannels;

        uint8_t *data[6];
        for (uint64_t i = 0; i < 6; i++)
        {
            int nbc;
            data[i] = stbi_load(filenames[i].c_str(), &width[i], &height[i], &nbc, 0);

            if (data[i] == nullptr)
                throw runtime_error("Failed to load texture file '" + filenames[i] + "'");

            if (i > 0 && (width[i-1] != width[i] || height[i-1] != height[i] || nbChannels != nbc))
                throw runtime_error("Incoherent values between images");

            nbChannels = nbc;
        }

        const auto mainFunc = [texture, width, height, nbChannels, data]()
        {

            glGenTextures(1, &texture->id);

            // Bind texture and set filtering
            texture->setFiltering(GL_LINEAR, GL_LINEAR);

            GLenum format;
            switch (nbChannels)
            {
                case 1:
                    format = GL_RED;
                    break;
                case 2:
                    format = GL_RG;
                    break;
                case 3:
                    format = GL_RGB;
                    break;
                case 4:
                    format = GL_RGBA;
                    break;
                default:
                    throw runtime_error("Unsupported number of channels: " + to_string(nbChannels));
            }

            for (size_t i = 0; i < 6; i++)
            {
                glTexImage2D(
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        0, format, width[i], height[i], 0, format, GL_UNSIGNED_BYTE, data[i]
                );
                stbi_image_free(data[i]);
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        };

        Dispatcher::instance().enqueueMainDelayed(mainFunc);

    };

    Dispatcher::instance().io()->enqueue(func);

    return texture;
}
