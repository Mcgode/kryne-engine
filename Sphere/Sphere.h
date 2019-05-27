//
// Created by max on 21/05/19.
//

#ifndef INC_3D_DEMOS_SPHERE_H
#define INC_3D_DEMOS_SPHERE_H


#include <common/BaseObject.h>
#include <common/VertexArray.h>
#include <common/GeometricShapes.h>
#include <common/Texture2D.h>

class Sphere : public BaseObject {

public:
    explicit Sphere();

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

private:

    Texture2D *diffuseTexture;

};


#endif //INC_3D_DEMOS_SPHERE_H
