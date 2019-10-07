//
// Created by max on 02/08/2019.
//

#ifndef INC_3D_DEMOS_TEXTURECUBEMAP_H
#define INC_3D_DEMOS_TEXTURECUBEMAP_H


#include <common/Texture2D.h>

class TextureCubeMap: public Texture2D {

public:

    TextureCubeMap(const std::string &folderName, const std::string &fileExtension);

    void setTexture(Shader *shader, const std::string &uniformName) override;

};


#endif //INC_3D_DEMOS_TEXTURECUBEMAP_H
