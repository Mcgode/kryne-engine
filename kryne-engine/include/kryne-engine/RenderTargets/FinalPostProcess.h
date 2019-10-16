//
// Created by max on 26/07/2019.
//

#ifndef INC_3D_DEMOS_FINAL_POST_PROCESS_H
#define INC_3D_DEMOS_FINAL_POST_PROCESS_H

#include <map>

#include <kryne-engine/Shader.h>
#include <kryne-engine/VertexArray.h>
#include <kryne-engine/GeometricShapes.h>
#include <kryne-engine/Texture2D.h>

using namespace std;


class FinalPostProcess {

public:

    explicit FinalPostProcess(Shader *shader);

    void setIntUniform(int value, const string& uniformName);

    void setFloatUniform(float value, const string& uniformName);

    void setVec2Uniform(glm::vec2 vector, const string& uniformName);

    void setVec3Uniform(glm::vec3 vector, const string& uniformName);

    void setVec4Uniform(glm::vec4 vector, const string& uniformName);

    void setTextureUniform(Texture2D *textureUniform, const string& uniformName);

    virtual void draw();

    ~FinalPostProcess();

protected:

    Shader *postProcessShader;
    VertexArray *va{};

    uint8_t nextIndex{0};
    map<string, uint8_t> textures{};

};


#endif //INC_3D_DEMOS_FINAL_POST_PROCESS_H
