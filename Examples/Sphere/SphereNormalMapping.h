//
// Created by max on 21/05/19.
//

#ifndef INC_KRYNE_ENGINE_SPHERE_NORMAL_MAPPING_H
#define INC_KRYNE_ENGINE_SPHERE_NORMAL_MAPPING_H


#include "SphereTexture.h"

class SphereNormalMapping : public SphereTexture {

public:
    explicit SphereNormalMapping(const std::string &textureDirName);

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

    ~SphereNormalMapping() override;

private:

    Texture2D *normalMapTexture;

};


#endif //INC_KRYNE_ENGINE_SPHERE_NORMAL_MAPPING_H
