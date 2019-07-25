//
// Created by max on 22/05/19.
//

#ifndef INC_3D_DEMOS_TEXTURE2D_H
#define INC_3D_DEMOS_TEXTURE2D_H

#include <common/stb_image.h>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

class Texture2D {

public:
    explicit Texture2D(const std::string& filename, bool generateMipMap = false);

    static void textureSet(int textureID, Shader *shader, GLuint textureIndex = 0, const std::string &uniformName = "");

    static Texture2D *getTexture(const std::string &directoryName, const std::string &filenameSuffix = "diff");

    void setTexture(Shader *shader, GLuint textureIndex = 0, const std::string &uniformName = "");

private:

    int width{};
    int height{};
    int nbChannels{};

    GLuint textureId{};

};


#endif //INC_3D_DEMOS_TEXTURE2D_H
