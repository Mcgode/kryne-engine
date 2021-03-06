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
    this->programID = 0;

    this->needsUpdate = SHADER_VERTEX_NEEDS_UPDATE | SHADER_FRAGMENT_NEEDS_UPDATE;

    this->uniformsHandler = make_unique<UniformsHandler>(&this->programID);

    maxIndex = 0;
}


Shader::Shader(const char *vertexShaderFilename, const char *fragmentShaderFilename): Shader()
{
    // Recovering and compiling both shaders
    loadShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilename, &this->vertexShader);
    loadShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilename, &this->fragmentShader);
}


Shader::~Shader()
{
    glDeleteProgram(this->programID);
}


void Shader::loadShaderFromFile(GLenum type, const char *filename, std::string *shaderCode)
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
    string line;
    while (Utils::safeGetLine(file, line)) {
        if (!shaderCode->empty())
            shaderCode->push_back('\n');
        *shaderCode += line;
    }
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


void Shader::linkProgram(const GLuint &vertex, const GLuint &fragment)
{
    assertIsMainThread();

    if (this->programID == 0)
        this->programID = glCreateProgram();

    // Attaching shaders to program
    if (this->previousVertex != vertex)
        glAttachShader(this->programID, vertex);
    this->previousVertex = vertex;

    if (this->previousFragment != fragment)
        glAttachShader(this->programID, fragment);
    this->previousFragment = fragment;

    // Linking program
    glLinkProgram(this->programID);

    GLint success;
    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if(!success) {
        string infoLog;
        infoLog.resize(2048);
        glGetProgramInfoLog(this->programID, infoLog.size(), nullptr, infoLog.data());
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}


string Shader::makeDefinesCode()
{
    string code;
    {
        scoped_lock<mutex> l(this->mapMutex);

        for (const auto &pair : this->defines)
            code += "#define " + pair.first + " " + pair.second + "\n";
    }
    return code;
}


void Shader::debugPrintActiveAttributes() const
{
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 64; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length;

    glGetProgramiv(this->programID, GL_ACTIVE_ATTRIBUTES, &count);
    printf("Active Attributes: %d\n", count);

    for (i = 0; i < count; i++)
    {
        glGetActiveAttrib(this->programID, (GLuint)i, bufSize, &length, &size, &type, name);

        printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
    }

    printf("Done");
}


void Shader::debugPrintActiveUniforms() const
{
    assertIsMainThread();

    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 64; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length;

    glGetProgramiv(this->programID, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);

    for (i = 0; i < count; i++)
    {
        glGetActiveUniform(this->programID, (GLuint)i, bufSize, &length, &size, &type, name);

        printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
    }
}


void Shader::setDefine(const string &defineName, const string &defineValue)
{
    bool changed;
    {
        scoped_lock<mutex> l(this->mapMutex);
        const auto emplaceResult = this->defines.emplace(defineName, defineValue);

        changed = emplaceResult.second || emplaceResult.first->second != defineValue;

        if (!emplaceResult.second && changed)
            emplaceResult.first->second = defineValue;
    }

    if (changed)
        this->needsUpdate |= SHADER_VERTEX_NEEDS_UPDATE | SHADER_FRAGMENT_NEEDS_UPDATE;
}


bool Shader::removeDefine(const string &defineName)
{
    bool erased;
    {
        scoped_lock<mutex> l(this->mapMutex);
        erased = this->defines.erase(defineName) > 0;
    }

    if (erased)
        this->needsUpdate |= SHADER_VERTEX_NEEDS_UPDATE | SHADER_FRAGMENT_NEEDS_UPDATE;

    return erased;
}
