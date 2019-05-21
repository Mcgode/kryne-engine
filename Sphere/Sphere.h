//
// Created by max on 21/05/19.
//

#ifndef INC_3D_DEMOS_SPHERE_H
#define INC_3D_DEMOS_SPHERE_H


#include <common/BaseObject.h>
#include <common/VertexArray.h>
#include <common/GeometricShapes.h>

class Sphere : public BaseObject {

public:
    explicit Sphere(Shader *shader);

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

};


#endif //INC_3D_DEMOS_SPHERE_H
