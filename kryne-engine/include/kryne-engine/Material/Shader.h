/**
 * @file Shader.h
 * @author Max Godefroy
 * @date 02/08/2017
 */

#ifndef GLFW_TRAINING_SHADER_H
#define GLFW_TRAINING_SHADER_H

#include <map>
#include <sstream>

#include <kryne-engine/Dispatch/Dispatcher.h>

#include "UniformsHandler.h"
#include "ShaderChunk.h"

#define SHADER_VERTEX_NEEDS_UPDATE      0b001u
#define SHADER_FRAGMENT_NEEDS_UPDATE    0b010u

using namespace std;


/**
 * Handles the storage of shader information
 */
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
        Shader::setVertexShader(vertexShader);
        Shader::setFragmentShader(fragmentShader);
    }

    /**
     * Delete the shader and the associated shader program.
     */
    ~Shader();

    /**
     * Links the provided shaders to the program
     * @param vertex    The vertex shader to attach to the program.
     * @param fragment  The fragment shader to attach to the program.
     */
    void linkProgram(const GLuint &vertex, const GLuint &fragment);

protected:

    /**
     * Protected initializer for the shader program.
     */
    Shader();

    /**
     * Loads a shader for the local disk.
     * @param type          The type of shader to load.
     * @param filename      The filename of the shader to load.
     * @param shaderCode    The string where the shader code will be stored.
     */
    static void loadShaderFromFile(GLenum type, const char *filename, std::string *shaderCode);


public:

    /**
     * Notify renderer to use this shader for drawing.
     *
     * @note Call this when the shader has finished updating and/or compiling since Shader::needsUpdate will be reset.
     */
    void use();

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
     * Updates the vertex shader, and marks the shader for recompiling
     * @param newVertexShader   The new vertex shader code.
     */
    void setVertexShader(const string &newVertexShader) {
        Shader::vertexShader = newVertexShader;
        Shader::needsUpdate |= SHADER_VERTEX_NEEDS_UPDATE;
    }


    /**
     * Retrieve current fragment shader code
     */
    [[nodiscard]] const string &getFragmentShader() const {
        return fragmentShader;
    }

    /**
     * Updates the fragment shader, and marks the shader for recompiling
     * @param newFragmentShader  The new fragment shader code.
     */
    void setFragmentShader(const string &newFragmentShader) {
        Shader::fragmentShader = newFragmentShader;
        Shader::needsUpdate |= SHADER_VERTEX_NEEDS_UPDATE;
    }

    /**
     * Retrieves need-update status.
     */
    [[nodiscard]] uint8_t getNeedsUpdate() const { return needsUpdate; }

private:

    GLuint programID;

    GLuint previousVertex = 0;

    GLuint previousFragment = 0;

    uint8_t needsUpdate = 0b00;

    string vertexShader;

    string fragmentShader;

    std::map<std::string, uint8_t> textureMap{};

    uint8_t maxIndex{};


public:

    /**
     * Retrieves the current list of defines for this shader
     */
    [[nodiscard]] const unordered_map<string, string> &getDefines() const { return defines; }

    /**
     * Sets a define constant value for the shader.
     * @param defineName    The name of the constant.
     * @param defineValue   The value of the constant as a string. Can be empty, in case you just want to define an
     *                      empty constant to use #ifdef and the like.
     */
    void setDefine(const string &defineName, const string &defineValue);

    /**
     * Removes a define constant from the list
     * @param defineName    The name of the constant to remove
     * @returns true if an element was erased, false otherwise.
     */
    bool removeDefine(const string &defineName);

    /**
     * Generates the code for the defines
     */
    [[nodiscard]] string makeDefinesCode();

private:

    /// The storage for the defines
    unordered_map<string, string> defines;

    /// A mutex for define writing
    mutex mapMutex;


public:

    void setTexture(const std::string &name);

    //! @copydoc UniformsHandler::setUniform(const string &, const UniformsHandler::UniformTypes &)
    void setUniform(const string &name, const UniformsHandler::UniformTypes &value) const { Shader::uniformsHandler->setUniform(name, value); }

    //! @copydoc UniformsHandler::setUniform(const string &, const UniformsHandler::UniformTypes &, uint32_t)
    void setUniform(const string &name, const UniformsHandler::UniformTypes &value, uint32_t arrayIndex) const { Shader::uniformsHandler->setUniform(name, value, arrayIndex); }

    //! @copydoc UniformsHandler::removeUniform()
    [[nodiscard]] bool removeUniform(const string &name) const { return Shader::uniformsHandler->removeUniform(name); }

    //! @copydoc UniformsHandler::notifyUniformLocationsNeedUpdate
    void notifyUniformLocationsNeedUpdate() { this->uniformsHandler->notifyUniformLocationsNeedUpdate(); }

    //! @copydoc UniformsHandler::updateUniforms()
    void updateUniforms() const { Shader::uniformsHandler->updateUniforms(); }

private:

    void setInt(const std::string &name, int value) const;

    unique_ptr<UniformsHandler> uniformsHandler {};


public:

    /**
     * Prints the active attributes of the shader, and their locations
     */
    void debugPrintActiveAttributes() const;

    /**
     * Prints the active uniforms of the shader, and their locations
     */
    void debugPrintActiveUniforms() const;

};


#endif //GLFW_TRAINING_SHADER_H
