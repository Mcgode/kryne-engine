//
// Created by max on 22/05/19.
//

#ifndef INC_3D_DEMOS_TEXTURE2D_H
#define INC_3D_DEMOS_TEXTURE2D_H

#include <common/stb_image.h>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

class Texture2D {

public:
    explicit Texture2D(const std::string& filename, bool generateMipMap = false);

    static void textureSet(int textureID, Shader *shader, GLuint textureIndex, const std::string *uniformName);

    void setTexture(Shader *shader, GLuint textureIndex, std::string *uniformName);

private:

    int width{};
    int height{};
    int nbChannels{};

    GLuint textureId{};

};


#endif //INC_3D_DEMOS_TEXTURE2D_H
