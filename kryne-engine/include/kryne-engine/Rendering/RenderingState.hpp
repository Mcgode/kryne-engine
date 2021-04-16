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
        setScissor(false);
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
        if (!glm::all(glm::equal(newSize, this->viewportSize)))
            glViewport(this->viewportStart.x, this->viewportStart.y, newSize.x, newSize.y);
        RenderingState::viewportSize = newSize;
    }

    void setViewport(const glm::ivec2 &start, const glm::ivec2 &size)
    {
        if (!glm::all(glm::equal(size, this->viewportSize)) || !glm::all(glm::equal(start, this->viewportStart)))
            glViewport(start.x, start.y, size.x, size.y);

        this->viewportStart = start;
        this->viewportSize = size;
    }

private:

    glm::ivec2 viewportStart {};

    glm::ivec2 viewportSize {};


// --- Scissoring ---

public:

    /// @brief Retrieves scissor rectangle start
    [[nodiscard]] const glm::ivec2 &getScissorStart() const { return scissorStart; }

    /// @brief Updates scissor rectangle start
    void setScissorStart(const glm::ivec2 &newStart)
    {
        if (!glm::all(glm::equal(newStart, this->scissorStart)))
            glScissor(newStart.x, newStart.y, this->scissorSize.x, this->scissorSize.y);
        RenderingState::scissorStart = newStart;
    }

    /// @brief Retrieves scissor rectangle size
    [[nodiscard]] const glm::ivec2 &getScissorSize() const { return scissorSize; }

    /// @brief Updates scissor rectangle size
    void setScissorSize(const glm::ivec2 &newSize)
    {
        if (!glm::all(glm::equal(newSize, this->scissorSize)))
            glScissor(this->scissorStart.x, this->scissorStart.y, newSize.x, newSize.y);
        RenderingState::scissorSize = newSize;
    }

    /// @brief Updates scissor rectangle
    void setScissorValues(const glm::ivec2 &start, const glm::ivec2 &size)
    {
        if (!glm::all(glm::equal(size, this->scissorSize)) || !glm::all(glm::equal(start, this->scissorStart)))
            glScissor(start.x, start.y, size.x, size.y);

        this->scissorStart = start;
        this->scissorSize = size;
    }

    /// @brief Toggles the scissoring test, and will update GL accordingly.
    void setScissor(bool value)
    {
        if (value != scissorEnable)
        {
            if (value)
                glEnable(GL_SCISSOR_TEST);
            else
                glDisable(GL_SCISSOR_TEST);
        }
        this->scissorEnable = value;
    }

private:

    /// Whether scissor test is enabled or not
    bool scissorEnable = false;

    /// The scissoring rectangle start
    glm::ivec2 scissorStart {};

    /// The scissoring rectangle size
    glm::ivec2 scissorSize {};

};


#endif //INC_KRYNE_ENGINE_RENDERINGSTATE_HPP
