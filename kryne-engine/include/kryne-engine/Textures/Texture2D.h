//
// Created by max on 22/05/19.
//

#ifndef INC_3D_DEMOS_TEXTURE2D_H
#define INC_3D_DEMOS_TEXTURE2D_H

#include <kryne-engine/Textures/stb_image.h>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "kryne-engine/Material/Shader.h"

class Texture2D {

public:
    explicit Texture2D(const std::string& filename, bool generateMipMap = false);

    explicit Texture2D(Texture2D *texture);

    static void
    textureSet(int textureID, Shader *shader, const std::string &uniformName, GLenum textureType = GL_TEXTURE_2D);

    static Texture2D *getTexture(const std::string &directoryName, const std::string &filenameSuffix = "diff");

    virtual void setTexture(Shader *shader, const std::string &uniformName);

    virtual ~Texture2D();

protected:

    Texture2D();

    int width{};
    int height{};
    int nbChannels{};

    GLuint textureId{};
    bool originalTexture = true;

};


#endif //INC_3D_DEMOS_TEXTURE2D_H
