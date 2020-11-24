/**
 * @file
 * @author Max Godefroy
 * @date 23/11/2020
 */

#ifndef INC_3D_DEMOS_SHADERCHUNK_H
#define INC_3D_DEMOS_SHADERCHUNK_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


/**
 * A singleton class for handling the retrieval and caching of shader code chunks
 */
class ShaderChunk {

public:

    /**
     * Get the ShaderChunk instance.
     *
     * @note Thread safety of the initialization operation is assured by using a Meyers singleton pattern.
     */
    static ShaderChunk &getInstance() {
        static ShaderChunk instance;
        return instance;
    }

    /**
     * Retrieves the shader code chunk corresponding to the provided filename.
     *
     * @param filename  The name of the shader file.
     * @return A string containing the shader code.
     *
     * @note Uses a cache system to avoid repeated access cost due to IO operations.
     *
     * @warning Might not be a thread safe operation
     */
    const string &getCodeChunk(const string &filename);

private:

    /**
     * Perform IO operation to retrieve the code chunk, when encountering a cache miss.
     * @param filename  The name of the file to retrieve from.
     * @return A string containing the shader code.
     */
    static string loadCodeFile(const string &filename);

private:

    /**
     * The cache map for all the retrieved code chunks.
     *
     * @note Since the shader chunks are a static set of files, memory isn't much of a problem, and, as such, a cache
     *       expiry system was deemed unnecessary.
     */
    unordered_map<string, string> shaderCodeCache {};

};


#endif //INC_3D_DEMOS_SHADERCHUNK_H
