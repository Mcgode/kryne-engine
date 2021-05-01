/**
 * @file
 * @author Max Godefroy
 * @date 22/05/19
 */

#ifndef INC_KRYNE_ENGINE_TEXTURE2D_H
#define INC_KRYNE_ENGINE_TEXTURE2D_H

#include "Texture.h"


using namespace std;

class Texture2D: public Texture {

public:

    explicit Texture2D(GLuint id): Texture(id, GL_TEXTURE_2D) {};

    static shared_ptr<Texture2D> loadFromFile(const string &filename);

protected:

    explicit Texture2D(const std::string& filename, bool generateMipMap = false);

};


#endif //INC_KRYNE_ENGINE_TEXTURE2D_H
