//
// Created by Max Godefroy on 02/08/2017.
//

#ifndef GLFW_TRAINING_SHADER_H
#define GLFW_TRAINING_SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {

public:
    Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    Shader(const char *shaderName) : Shader(shaderName, shaderName) {};
    ~Shader();

    void use();
    static void resetUse();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec3(const std::string &name, glm::vec3 vec);
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat4(const std::string &name, glm::mat4 mat) const;
    void setMat3(const std::string &name, glm::mat3 mat) const;

    unsigned int ID;

private:
    unsigned int createShaderFromFile(GLenum type, const char *filename);

};


#endif //GLFW_TRAINING_SHADER_H
