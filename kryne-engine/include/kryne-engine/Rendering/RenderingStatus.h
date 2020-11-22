//
// Created by Max Godefroy on 22/11/2020.
//

#ifndef INC_3D_DEMOS_RENDERINGSTATUS_H
#define INC_3D_DEMOS_RENDERINGSTATUS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <kryne-engine/enums/MaterialSide.h>

class RenderingStatus {

public:

    explicit RenderingStatus(MaterialSide baseSide) : side(baseSide) {}

    [[nodiscard]] MaterialSide getSide() const {
        return side;
    }

    void setSide(MaterialSide newBaseSide) {
        RenderingStatus::side = newBaseSide;
        RenderingStatus::updateToSide(newBaseSide);
    }

    static void updateToSide(MaterialSide side) {
        if (side < DoubleSide) {
            glEnable(GL_CULL_FACE);
            glCullFace(side == FrontSide ? GL_BACK : GL_FRONT);
        } else
            glDisable(GL_CULL_FACE);
    }

private:

    MaterialSide side;

};


#endif //INC_3D_DEMOS_RENDERINGSTATUS_H
