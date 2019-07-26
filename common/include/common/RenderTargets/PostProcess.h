//
// Created by max on 25/07/2019.
//

#ifndef INC_3D_DEMOS_POST_PROCESS_H
#define INC_3D_DEMOS_POST_PROCESS_H

#include <map>
#include <string>

#include "common/Shader.h"
#include "common/VertexArray.h"
#include "common/Texture2D.h"


using namespace std;

/**
 * A class for handling post-processing tools
 */
class PostProcess{

public:

    /**
     * A class for handling post-processing tools
     * @param shader    The post-processing shader
     * @param width     The width of the viewport
     * @param height    The height of the viewport
     * @param channels  The color channels for the export texture
     * @param type      The data type of the texture
     */
    PostProcess(Shader *shader, uint32_t width, uint32_t height, GLenum channels, GLenum type);

    void setIntUniform(int value, const string& uniformName);

    void setFloatUniform(float value, const string& uniformName);

    void setVec2Uniform(glm::vec2 vector, const string& uniformName);

    void setVec3Uniform(glm::vec3 vector, const string& uniformName);

    void setVec4Uniform(glm::vec4 vector, const string& uniformName);

    void setTextureUniform(Texture2D *textureUniform, const string& uniformName);

    GLuint getTexture() const;

    void draw();


private:

    Shader *postProcessShader;
    GLuint fbo{};
    GLuint texture{};
    VertexArray *va{};

    uint8_t nextIndex{0};
    map<string, uint8_t> textures{};

};


#endif //INC_3D_DEMOS_POST_PROCESS_H
