//
// Created by Max Godefroy on 02/08/2017.
//

#include "common/Shader.h"

#ifndef _WIN32
#define DIR "Shaders/"
#else
#define DIR "shaders\\"
#endif


Shader::Shader(const char *vertexShaderFilename, const char *fragmentShaderFilename)
{
    // Recovering and compiling both shaders
    unsigned int vertex = createShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilename),
            fragment = createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilename);

    // Initialising program
    ID = glCreateProgram();

    // Attaching shaders to program
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    // Linking program
    glLinkProgram(ID);

    GLint success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    // Deleting shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


Shader::~Shader()
{
    glDeleteProgram(ID);
}


unsigned int Shader::createShaderFromFile(GLenum type, const char *filename)
{
    // Initialising shader from type
    unsigned int shader = glCreateShader(type);

    // Loading extension from shader type
    std::string extension;
    switch (type) {
        case GL_FRAGMENT_SHADER:
            extension = ".frag";
            break;
        case GL_VERTEX_SHADER:
            extension = ".vert";
        default:
            break;
    }

    // Saving full filename
    std::string fullFilename = DIR;
    fullFilename += filename + extension;

    // Opening read stream
    std::ifstream file(fullFilename);

    // Check if file was found
    if (!file) {
        std::cerr << "ERROR::SHADER::SHADER_PROGRAM_RECOVERY_FAILED\nCould not find file at " << fullFilename.c_str() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Parsing source code into a string
    std::string sourceCode;
    file.seekg(0, std::ios::end); // Setting cursor position to file end
    sourceCode.resize(file.tellg()); // Getting value of last position from the current cursor position and setting the string size accordingly
    file.seekg(0, std::ios::beg); // Resetting the cursor position to the beginning
    file.read(&sourceCode[0], sourceCode.size()); // Copying whole file to string
    file.close();
    const GLchar* sourceCodeString = sourceCode.c_str();

    // Compiling shader
    glShaderSource(shader, 1, &sourceCodeString, nullptr);
    glCompileShader(shader);

    // Check if compile succeeded
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success == 0)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}


void Shader::use()
{
    glUseProgram(ID);
}


void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}


void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}


void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}


void Shader::setMat4(const std::string &name, glm::mat4 mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, glm::vec3 vec) {
    setVec3(name, vec.x, vec.y, vec.z);
}


/**
 * Resets the current shader program to the base program.
 */
void Shader::resetUse()
{
    glUseProgram(0);
}


void Shader::setMat3(const std::string &name, glm::mat3 mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}


void Shader::setVec2(const std::string &name, glm::vec2 vec) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y);
}


void Shader::setVec4(const std::string &name, glm::vec4 vec)
{
    setVec4(name, vec.x, vec.y, vec.z, vec.w);
}
