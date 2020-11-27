/**
 * @file
 * @author Max Godefroy
 * @date 27/11/2020.
 */

#ifndef INC_3D_DEMOS_SHADERPROGRAMCOMPILER_H
#define INC_3D_DEMOS_SHADERPROGRAMCOMPILER_H

#include <kryne-engine/Material/Shader.h>
#include <kryne-engine/Geometry/BufferGeometry.h>


/// The default GLSL version that is put at the start of every new shader
#define BASE_GLSL_VERSION_STRING std::string("#version 450 core")


/**
 * Handles shader program dynamic compilation
 */
class ShaderProgramCompiler {

public:

    ShaderProgramCompiler();

    void checkShaderCompile(const Shader *shader, const BufferGeometry *geometry);

protected:

    static string replaceIncludes(const string &baseCode, const string &indentation = "");

    static void compileShader(const string &code, const GLuint &id);

    void compileVertex(const string &shaderCode, const string &definesCode) const;

    void compileFragment(const string &shaderCode, const string &definesCode) const;

protected:

    bool needsUpdate = false;

    GLuint vertexShaderId = 0;

    GLuint fragmentShaderId = 0;

    pair<string, uint64_t> previousLayoutCode {};

};


#endif //INC_3D_DEMOS_SHADERPROGRAMCOMPILER_H
