//
// Created by max on 09/06/19.
//

#ifndef INC_KRYNE_ENGINE_SPHEREPARALLAX_H
#define INC_KRYNE_ENGINE_SPHEREPARALLAX_H


#include "SphereNormalMapping.h"

class SphereParallax: public SphereNormalMapping {

public:

    explicit SphereParallax(const std::string& textureDirName);

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

    virtual ~SphereParallax();

private:

    Texture2D *displacementMap{};

};


#endif //INC_KRYNE_ENGINE_SPHEREPARALLAX_H
