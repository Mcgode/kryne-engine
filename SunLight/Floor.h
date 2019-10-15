//
// Created by max on 26/07/2019.
//

#ifndef INC_3D_DEMOS_FLOOR_H
#define INC_3D_DEMOS_FLOOR_H


#include <common/3DObjects/BaseObject.h>
#include <common/Texture2D.h>

class Floor: public BaseObject {

public:

    explicit Floor(const std::string &textureName, Scene *scene, DirectionalLight *light);

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

    virtual ~Floor();

protected:

    Texture2D *diffuseTexture;
    Texture2D *normalMapTexture;

    ShadowMapHandler *handler;
    DirectionalLight *light;

};


#endif //INC_3D_DEMOS_FLOOR_H
