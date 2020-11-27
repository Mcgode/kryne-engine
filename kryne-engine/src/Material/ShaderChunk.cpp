//
// Created by Max Godefroy on 23/11/2020.
//

#include "kryne-engine/Material/ShaderChunk.h"

#ifndef _WIN32
#define DIR "Resources/Shaders/Engine/ShaderChunk/"
#else
#define DIR "Resources\\Shaders\\Engine\\ShaderChunk\\"
#endif


const string &ShaderChunk::getCodeChunk(const string &filename)
{
    auto location = this->shaderCodeCache.find(filename);
    if (location != this->shaderCodeCache.end())
        return location->second;
    auto code = loadCodeFile(filename);
    this->shaderCodeCache.emplace(filename, code);
    return this->shaderCodeCache.at(filename);
}


string ShaderChunk::loadCodeFile(const string &filename) {

    string shaderCode;

    // Saving full filename
    string fullFilename = DIR + filename + ".glsl";

    // Opening read stream
    ifstream file(fullFilename);

    // Check if file was found
    if (!file) {
        std::cerr << "ERROR::SHADER::SHADER_PROGRAM_RECOVERY_FAILED\nCould not find file at " << fullFilename.c_str() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Parsing source code into a string
    string line;
    while (StringUtils::safeGetLine(file, line)) {
        if (!shaderCode.empty())
            shaderCode.push_back('\n');
        shaderCode += line;
    }
    file.close();

    return shaderCode;
}
