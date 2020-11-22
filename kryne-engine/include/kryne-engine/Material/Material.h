//
// Created by Max Godefroy on 21/11/2020.
//

#ifndef INC_3D_DEMOS_MATERIAL_H
#define INC_3D_DEMOS_MATERIAL_H

#include <memory>
#include <kryne-engine/enums/MaterialSide.h>
#include "Shader.h"

using namespace std;


class Material {

public:

    explicit Material(unique_ptr<Shader> shader) : shader(move(shader)) {}

    [[nodiscard]] const Shader *getShader() const {
        return shader.get();
    }

    void setShader(unique_ptr<Shader> newShader) {
        Material::shader = move(newShader);
    }

    void use();

    void resetUse();

private:

    unique_ptr<Shader> shader;


public:

    [[nodiscard]] GLenum getPrimitiveType() const {
        return primitiveType;
    }

    void setPrimitiveType(GLenum newPrimitiveType) {
        Material::primitiveType = newPrimitiveType;
    }

    [[nodiscard]] MaterialSide getSide() const {
        return side;
    }

    void setSide(MaterialSide newSide) {
        Material::side = newSide;
    }

private:

    GLenum primitiveType = GL_TRIANGLES;

    MaterialSide side = FrontSide;

};


#endif //INC_3D_DEMOS_MATERIAL_H
