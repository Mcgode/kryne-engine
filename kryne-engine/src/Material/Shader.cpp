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
    // Initialise shaders
    this->vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    this->fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Initialise program
    this->programID = glCreateProgram();

    maxIndex = 0;
}


Shader::Shader(const char *vertexShaderFilename, const char *fragmentShaderFilename): Shader()
{
    // Recovering and compiling both shaders
    createShaderFromFile(this->vertexShaderId,   GL_VERTEX_SHADER,   vertexShaderFilename,   &this->vertexShader);
    createShaderFromFile(this->fragmentShaderId, GL_FRAGMENT_SHADER, fragmentShaderFilename, &this->fragmentShader);

    this->compileProgram();
}


Shader::~Shader()
{
    glDeleteShader(this->vertexShaderId);
    glDeleteShader(this->fragmentShaderId);
    glDeleteProgram(this->programID);
}


void Shader::createShaderFromFile(GLuint shader, GLenum type, const char *filename, std::string *shaderCode)
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

    Shader::compileShader(shader, shaderCode);
}


void Shader::use()
{
    if (this->needsUpdate) {
        if (this->needsUpdate & VERTEX_SHADER_NEEDS_UPDATE)
            this->compileShader(this->vertexShaderId, &this->vertexShader);
        if (this->needsUpdate & FRAGMENT_SHADER_NEEDS_UPDATE)
            this->compileShader(this->fragmentShaderId, &this->fragmentShader);
        this->compileProgram();
        this->needsUpdate = 0;
    }
    glUseProgram(this->programID);
}


void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}


void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}


void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}


void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}


void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, w);
}


void Shader::setMat4(const std::string &name, glm::mat4 mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, glm::vec3 vec) const {
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
    glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}


void Shader::setVec2(const std::string &name, glm::vec2 vec) const
{
    glUniform2f(glGetUniformLocation(programID, name.c_str()), vec.x, vec.y);
}


void Shader::setVec4(const std::string &name, glm::vec4 vec) const
{
    setVec4(name, vec.x, vec.y, vec.z, vec.w);
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


void Shader::compileShader(GLuint shader, const string *code)
{
    string finalCode = "#version 330 core\n";
    finalCode += this->makeDefinesCode();
    finalCode += Shader::replaceIncludes(*code);
    const GLchar* sourceCodeString = finalCode.c_str();

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
        cout << finalCode << endl;
    }
}


void Shader::compileProgram() const
{
    // Attaching shaders to program
    glAttachShader(this->programID, this->vertexShaderId);
    glAttachShader(this->programID, this->fragmentShaderId);

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


string Shader::replaceIncludes(const string &baseCode, const string &indentation)
{
    regex includeRegex(R"((\n|^)(\s*)(#include\s*<([a-zA-Z0-9_-]+)>))");
    smatch results;

    string newCode, line;
    istringstream iss(baseCode);
    while (getline(iss, line)) {
        if (!newCode.empty())
            newCode.push_back('\n');
        if (regex_search(line, results, includeRegex)) {
            auto codeChunk = ShaderChunk::getInstance().getCodeChunk(results[4].str());
            newCode += Shader::replaceIncludes(codeChunk, indentation + results[2].str());
        } else {
            newCode += indentation + line;
        }
    }

    return newCode;
}


string Shader::makeDefinesCode()
{
    string code;
    for (const auto &pair : this->defines) {
        code += "#define " + pair.first + " " + pair.second + "\n";
    }
    return code;
}
