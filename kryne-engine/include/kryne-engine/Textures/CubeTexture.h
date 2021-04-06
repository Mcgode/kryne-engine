/**
 * @file
 * @author Max Godefroy
 * @date 02/08/2019
 */

#ifndef INC_KRYNE_ENGINE_TEXTURECUBEMAP_H
#define INC_KRYNE_ENGINE_TEXTURECUBEMAP_H

#include "Texture.h"

using namespace std;


class CubeTexture: public Texture {

public:

    explicit CubeTexture(GLuint id): Texture(id, GL_TEXTURE_CUBE_MAP) {};

    static shared_ptr<CubeTexture> loadFiles(const vector<string> &filenames);

};


#endif //INC_KRYNE_ENGINE_TEXTURECUBEMAP_H
