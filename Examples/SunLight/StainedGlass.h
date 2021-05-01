//
// Created by max on 02/08/2019.
//

#ifndef INC_KRYNE_ENGINE_STAINEDGLASS_H
#define INC_KRYNE_ENGINE_STAINEDGLASS_H


#include <kryne-engine/3DObjects/BaseObject.h>
#include <kryne-engine/Textures/Texture2D.h>

class StainedGlass: public BaseObject {

public:
    StainedGlass();

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

private:

    Texture2D *texture{};

};


#endif //INC_KRYNE_ENGINE_STAINEDGLASS_H
