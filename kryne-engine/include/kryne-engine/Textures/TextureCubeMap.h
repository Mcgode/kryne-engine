/**
 * @file
 * @author Max Godefroy
 * @date 02/08/2019
 */

#ifndef INC_KRYNE_ENGINE_TEXTURECUBEMAP_H
#define INC_KRYNE_ENGINE_TEXTURECUBEMAP_H


#include <kryne-engine/Textures/Texture2D.h>

class TextureCubeMap: public Texture2D {

public:

    TextureCubeMap(const std::string &folderName, const std::string &fileExtension);

    void setTexture(Shader *shader, const std::string &uniformName) override;

};


#endif //INC_KRYNE_ENGINE_TEXTURECUBEMAP_H
