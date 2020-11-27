//
// Created by Max Godefroy on 02/08/2017.
//

#include "kryne-engine/Material/Shader.h"

#ifndef _WIN32
#define DIR "Resources/Shaders/"
#else
#define DIR "Resources\\Shaders\\"
#endif


Shader::Shader()
{
    // Initialise program
    this->programID = glCreateProgram();

    this->needsUpdate = SHADER_VERTEX_NEEDS_UPDATE | SHADER_FRAGMENT_NEEDS_UPDATE;

    this->uniformsHandler = make_unique<UniformsHandler>(this->programID);

    maxIndex = 0;
}


Shader::Shader(const char *vertexShaderFilename, const char *fragmentShaderFilename): Shader()
{
    // Recovering and compiling both shaders
    createShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilename, &this->vertexShader);
    createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilename, &this->fragmentShader);
}


Shader::~Shader()
{
    glDeleteProgram(this->programID);
}


void Shader::createShaderFromFile(GLenum type, const char *filename, std::string *shaderCode)
{
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
    file.seekg(0, std::ios::end); // Setting cursor position to file end
    shaderCode->resize(file.tellg()); // Getting value of last position from the current cursor position and setting the string size accordingly
    file.seekg(0, std::ios::beg); // Resetting the cursor position to the beginning
    file.read(&(*shaderCode)[0], shaderCode->size()); // Copying whole file to string
    file.close();
}


void Shader::use()
{
    this->needsUpdate = 0;
    glUseProgram(this->programID);
}


void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}


/**
 * Resets the current shader program to the base program.
 */
void Shader::resetUse()
{
    glUseProgram(0);
}


void Shader::setTexture(const std::string &name)
{

    auto pair = textureMap.find(name);
    if (pair == textureMap.end()) {
        textureMap.insert(std::pair<std::string, uint8_t>(name, maxIndex));
        glActiveTexture(GL_TEXTURE0 + maxIndex);
        setInt(name, maxIndex);
        maxIndex++;
    } else {
        glActiveTexture(GL_TEXTURE0 + pair->second);
        setInt(name, pair->second);
    }
}


void Shader::linkProgram(const GLuint &vertex, const GLuint &fragment) const
{
    // Attaching shaders to program
    glAttachShader(this->programID, vertex);
    glAttachShader(this->programID, fragment);

    // Linking program
    glLinkProgram(this->programID);

    GLint success;
    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(this->programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}


string Shader::makeDefinesCode() const
{
    string code;
    for (const auto &pair : this->defines) {
        code += "#define " + pair.first + " " + pair.second + "\n";
    }
    return code;
}
