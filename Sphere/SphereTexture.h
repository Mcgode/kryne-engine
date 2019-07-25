//
// Created by max on 09/06/19.
//

#ifndef INC_3D_DEMOS_SPHERETEXTURE_H
#define INC_3D_DEMOS_SPHERETEXTURE_H


#include "SphereBasic.h"

class SphereTexture: public SphereBasic {

public:

    explicit SphereTexture(const std::string &textureDirName);

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

private:

    Texture2D *diffuseTexture;

};


#endif //INC_3D_DEMOS_SPHERETEXTURE_H
