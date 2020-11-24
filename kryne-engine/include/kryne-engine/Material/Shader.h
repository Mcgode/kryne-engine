//
// Created by Max Godefroy on 02/08/2017.
//

#ifndef GLFW_TRAINING_SHADER_H
#define GLFW_TRAINING_SHADER_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <regex>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderChunk.h"

using namespace std;


class Shader {

public:

    /**
     * Initializes the shader from the provided shader files.
     * @param vertexShaderFilename      The filename of the vertex shader.
     * @param fragmentShaderFilename    The filename of the fragment shader.
     */
    Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename);

    /**
     * Initializes the shader from the provided shader files
     * @param shaderName    The filename of both the vertex and fragment shaders
     */
    explicit Shader(const char *shaderName) : Shader(shaderName, shaderName) {};

    /**
     * Initialize the shader by directly providing the shader code
     * @param vertexShader      The vertex shader code
     * @param fragmentShader    The fragment shader code
     */
    Shader(const string &vertexShader, const string &fragmentShader): Shader() {
        Shader::setVertexShader(vertexShader, false);
        Shader::setFragmentShader(fragmentShader, true);
    }

    /**
     * Delete the shader and the associated shader program.
     */
    ~Shader();

private:

    /**
     * Private initializer for initializing the shaders and the program.
     */
    Shader();

    void createShaderFromFile(GLuint shader, GLenum type, const char *filename, std::string *shaderCode);

    void compileShader(GLuint shader, const string *code);

    void compileProgram() const;


public:

    /**
     * Notify renderer to use this shader for drawing
     */
    void use() const;

    /**
     * Notify renderer to stop using this shader for drawing
     */
    static void resetUse();

    /**
     * Retrieve current vertex shader code
     */
    [[nodiscard]] const string &getVertexShader() const {
        return vertexShader;
    }

    /**
     * Updates the vertex shader, and optionally recompiles the entire program
     * @param newVertexShader   The new vertex shader code.
     * @param recompileProgram  Set to true to recompile the shader program. Defaults to true.
     */
    void setVertexShader(const string &newVertexShader, bool recompileProgram = true) {
        Shader::vertexShader = newVertexShader;
        Shader::compileShader(Shader::vertexShaderId, &newVertexShader);
        if (recompileProgram)
            Shader::compileProgram();
    }


    /**
     * Retrieve current fragment shader code
     */
    [[nodiscard]] const string &getFragmentShader() const {
        return fragmentShader;
    }

    /**
     * Updates the fragment shader, and optionally recompiles the entire program
     * @param newFragmentShader  The new fragment shader code.
     * @param recompileProgram   Set to true to recompile the shader program. Defaults to true.
     */
    void setFragmentShader(const string &newFragmentShader, bool recompileProgram = true) {
        Shader::fragmentShader = newFragmentShader;
        Shader::compileShader(Shader::fragmentShaderId, &newFragmentShader);
        if (recompileProgram)
            Shader::compileProgram();
    }

private:

    static string replaceIncludes(const string &baseCode, const string &indentation = "");

private:

    GLuint programID;

    string vertexShader;

    GLuint vertexShaderId;

    string fragmentShader;

    GLuint fragmentShaderId;

    std::map<std::string, uint8_t> textureMap{};

    uint8_t maxIndex{};


public:

    /**
     * Retrieves the current list of defines for this shader
     */
    [[nodiscard]] const unordered_map<string, string> &getDefines() const {
        return defines;
    }

    /**
     * Sets a define constant value for the shader.
     * @param defineName    The name of the constant.
     * @param defineValue   The value of the constant as a string. Can be empty, in case you just want to define an
     *                      empty constant to use #ifdef and the like.
     */
    void setDefine(const string &defineName, const string &defineValue) {
        const auto emplaceResult = Shader::defines.emplace(defineName, defineValue);
        if (!emplaceResult.second)
            emplaceResult.first->second = defineValue;
    }

    /**
     * Removes a define constant from the list
     * @param defineName    The name of the constant to remove
     * @returns true if an element was erased, false otherwise.
     */
    bool removeDefine(const string &defineName) {
        return Shader::defines.erase(defineName) > 0;
    }

private:

    /**
     * Generates the code for the defines
     */
    string makeDefinesCode();

    /// The storage for the defines
    unordered_map<string, string> defines;


public:

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setVec2(const std::string &name, glm::vec2 vec) const;

    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec3(const std::string &name, glm::vec3 vec) const;

    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    void setVec4(const std::string &name, glm::vec4 vec) const;

    void setMat4(const std::string &name, glm::mat4 mat) const;

    void setMat3(const std::string &name, glm::mat3 mat) const;

    void setTexture(const std::string &name);

};


#endif //GLFW_TRAINING_SHADER_H
