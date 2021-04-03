/**
 * @file
 * @author Max Godefroy
 * @date 22/11/2020
 */

#ifndef INC_KRYNE_ENGINE_RENDERINGSTATE_HPP
#define INC_KRYNE_ENGINE_RENDERINGSTATE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vector_relational.hpp>
#include <kryne-engine/enums/MaterialSide.h>

class RenderingState {


// --- Initialization

public:

    explicit RenderingState(const glm::ivec2 &viewportSize,
                            MaterialSide baseSide,
                            bool enableDepth = true,
                            const glm::ivec2 &viewportStart = glm::ivec2(0))
    {
        setSide(baseSide);
        setDepthTest(enableDepth);
        setDepthWrite(true);
        setViewportSize(viewportSize);
        setViewportStart(viewportStart);
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


// --- Viewport size ---

public:

    [[nodiscard]] const glm::ivec2 &getViewportStart() const { return viewportStart; }

    void setViewportStart(const glm::ivec2 &newStart)
    {
        if (!glm::all(glm::equal(newStart, this->viewportStart)))
            glViewport(newStart.x, newStart.y, this->viewportSize.x, this->viewportSize.y);
        RenderingState::viewportStart = newStart;
    }

    [[nodiscard]] const glm::ivec2 &getViewportSize() const { return viewportSize; }

    void setViewportSize(const glm::ivec2 &newSize)
    {
        if (!glm::all(glm::equal(newSize, this->viewportStart)))
            glViewport(this->viewportStart.x, this->viewportStart.y, newSize.x, newSize.y);
        RenderingState::viewportSize = newSize;
    }

private:

    glm::ivec2 viewportStart {};

    glm::ivec2 viewportSize {};

};


#endif //INC_KRYNE_ENGINE_RENDERINGSTATE_HPP
