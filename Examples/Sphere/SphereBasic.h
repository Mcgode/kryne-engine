//
// Created by max on 09/06/19.
//

#ifndef INC_3D_DEMOS_SPHEREBASIC_H
#define INC_3D_DEMOS_SPHEREBASIC_H


#include <kryne-engine/3DObjects/BaseObject.h>
#include <kryne-engine/VertexArray.h>
#include <kryne-engine/GeometricShapes.h>
#include <kryne-engine/Texture2D.h>

class SphereBasic : public BaseObject {

public:

    explicit SphereBasic();

    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params) override;

    ~SphereBasic() override;

protected:

    void initialize(Shader *shader);

};


#endif //INC_3D_DEMOS_SPHEREBASIC_H
