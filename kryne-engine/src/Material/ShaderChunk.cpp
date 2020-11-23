//
// Created by Max Godefroy on 23/11/2020.
//

#include "kryne-engine/Material/ShaderChunk.h"

#ifndef _WIN32
#define DIR "Resources/Shaders/Engine/"
#else
#define DIR "Resources\\Shaders\\Engine\\"
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
    file.seekg(0, std::ios::end); // Setting cursor position to file end
    shaderCode.resize(file.tellg()); // Getting value of last position from the current cursor position and setting the string size accordingly
    file.seekg(0, std::ios::beg); // Resetting the cursor position to the beginning
    file.read(&shaderCode[0], shaderCode.size()); // Copying whole file to string
    file.close();

    return shaderCode;
}
