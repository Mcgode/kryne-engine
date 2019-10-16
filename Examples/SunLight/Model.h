//
// Created by max on 29/07/2019.
//

#ifndef INC_3D_DEMOS_MODEL_H
#define INC_3D_DEMOS_MODEL_H

#include <common/3DObjects/Model/ModelLoader.h>
#include <common/Rendering/Scene.h>

class Model: public BaseObject{

public:

    Model(const string &model, const string &fallbackTexture, const Scene *scene, DirectionalLight *light);

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

private:
    vector<ModelMesh *> data{};
    Texture2D *diffuseMap;
    Texture2D *normalMap;

    ShadowMapHandler *handler;
    DirectionalLight *light;

};


#endif //INC_3D_DEMOS_MODEL_H
