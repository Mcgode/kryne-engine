//
// Created by Max Godefroy on 22/11/2020.
//

#ifndef INC_3D_DEMOS_RENDERINGSTATUS_HPP
#define INC_3D_DEMOS_RENDERINGSTATUS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <kryne-engine/enums/MaterialSide.h>

class RenderingStatus {

public:

    explicit RenderingStatus(MaterialSide baseSide, bool enableDepth = true) {
        setSide(baseSide);
        setDepthEnabled(enableDepth);
    }

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

    [[nodiscard]] bool isDepthEnabled() const {
        return depth;
    }

    void setDepthEnabled(bool enableDepth) {
        RenderingStatus::depth = enableDepth;
        if (enableDepth) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    }

private:

    MaterialSide side {};

    bool depth {};

};


#endif //INC_3D_DEMOS_RENDERINGSTATUS_HPP
