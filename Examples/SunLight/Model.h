//
// Created by max on 29/07/2019.
//

#ifndef INC_KRYNE_ENGINE_MODEL_H
#define INC_KRYNE_ENGINE_MODEL_H

#include <kryne-engine/3DObjects/Model/ModelLoader.h>
#include <kryne-engine/Rendering/RenderScene.h>

class Model: public BaseObject{

public:

    Model(const string &model, const string &fallbackTexture, const RenderScene *scene, OldDirectionalLight *light);

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

private:
    vector<ModelMesh *> data{};
    Texture2D *diffuseMap;
    Texture2D *normalMap;

    ShadowMapHandler *handler;
    OldDirectionalLight *light;

};


#endif //INC_KRYNE_ENGINE_MODEL_H
