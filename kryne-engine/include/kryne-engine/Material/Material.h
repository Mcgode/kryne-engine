//
// Created by Max Godefroy on 21/11/2020.
//

#ifndef INC_3D_DEMOS_MATERIAL_H
#define INC_3D_DEMOS_MATERIAL_H

#include <memory>
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

};


#endif //INC_3D_DEMOS_MATERIAL_H
