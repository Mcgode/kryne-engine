/**
 * @file
 * @author Max Godefroy
 * @date 27/11/2020.
 */

#ifndef INC_3D_DEMOS_SHADERPROGRAMCOMPILER_H
#define INC_3D_DEMOS_SHADERPROGRAMCOMPILER_H

#include <regex>
#include <kryne-engine/Material/Shader.h>
#include <kryne-engine/Geometry/BufferGeometry.h>


/// The default GLSL version that is put at the start of every new shader
#define BASE_GLSL_VERSION_STRING std::string("#version 450 core")


/**
 * Handles shader program dynamic compilation
 */
class ShaderProgramCompiler {

public:

    /**
     * Initializes the shaders
     */
    ShaderProgramCompiler();

    /**
     * Will check if a shader (re)compile is needed, and will proceed to do so.
     * @param shader    The shader information to compile.
     * @param geometry  The geometry that will be compiled.
     */
    void checkShaderCompile(const Shader *shader, const BufferGeometry *geometry);

protected:

    /**
     * Will unroll the includes of the provided code snippet.
     *
     * @param baseCode      The code to unroll.
     * @param indentation   The code block indentation.
     *
     * @return The new code, with all include calls replaced accordingly.
     */
    static string replaceIncludes(const string &baseCode, const string &indentation = "");

    /**
     * Compiles a shader.
     * @param code  The shader code.
     * @param id    The shader id.
     */
    static void compileShader(const string &code, const GLuint &id);

    /**
     * Prints prettily shader code to the console.
     * @param code  The code to print.
     */
    static void printCode(const string &code);

    /**
     * Compiles the vertex shader.
     * @param shaderCode    The vertex shader source code, retrieved from Shader::getVertexShader()
     * @param definesCode   The defines for this shader, retrieved from Shader::makeDefinesCode()
     */
    void compileVertex(const string &shaderCode, const string &definesCode) const;

    /**
     * Compiles the vertex shader.
     * @param shaderCode    The vertex shader source code, retrieved from Shader::getVertexShader()
     * @param definesCode   The defines for this shader, retrieved from Shader::makeDefinesCode()
     */
    void compileFragment(const string &shaderCode, const string &definesCode) const;

protected:

    /// The vertex shader OpenGL id
    GLuint vertexShaderId = 0;

    /// The fragment shader OpenGL id
    GLuint fragmentShaderId = 0;

    /// The layout code used in the current vertex shader.
    pair<string, uint64_t> previousLayoutCode {};

};


#endif //INC_3D_DEMOS_SHADERPROGRAMCOMPILER_H
