/**
 * @file
 * @author Max Godefroy
 * @date 22/11/2020
 */

#ifndef INC_3D_DEMOS_RENDERINGSTATE_HPP
#define INC_3D_DEMOS_RENDERINGSTATE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <kryne-engine/enums/MaterialSide.h>

class RenderingState {


// --- Initialization

public:

    explicit RenderingState(MaterialSide baseSide, bool enableDepth = true) {
        setSide(baseSide);
        setDepthTest(enableDepth);
        setDepthWrite(true);
    }


// --- Face culling ---

public:

    [[nodiscard]] MaterialSide getSide() const {
        return side;
    }

    void setSide(MaterialSide newBaseSide) {
        RenderingState::side = newBaseSide;
        RenderingState::updateToSide(newBaseSide);
    }

    static void updateToSide(MaterialSide side) {
        if (side < DoubleSide) {
            glEnable(GL_CULL_FACE);
            glCullFace(side == FrontSide ? GL_BACK : GL_FRONT);
        } else
            glDisable(GL_CULL_FACE);
    }

private:

    MaterialSide side {};


// --- Depth testing ---

public:

    [[nodiscard]] bool isDepthTestEnabled() const {
        return depthTest;
    }

    void setDepthTest(bool enableDepth) {
        RenderingState::depthTest = enableDepth;
        if (enableDepth) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    }

private:

    bool depthTest {};


// --- Depth writing ---

public:

    [[nodiscard]] bool isDepthWriteEnabled() const {
        return depthWrite;
    }

    void setDepthWrite(bool writeToDepth) {
        RenderingState::depthWrite = writeToDepth;
        glDepthMask(writeToDepth ? GL_TRUE : GL_FALSE);
    }

private:

    bool depthWrite {};

};


#endif //INC_3D_DEMOS_RENDERINGSTATE_HPP
