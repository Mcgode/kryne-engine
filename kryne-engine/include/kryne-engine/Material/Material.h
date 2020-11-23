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

    /**
     * Returns the primitive type currently used for drawing the mesh.
     */
    [[nodiscard]] GLenum getPrimitiveType() const {
        return primitiveType;
    }

    /**
     * Set the primitive type that will be used for drawing.
     * @param newPrimitiveType  The OpenGL draw primitive to use.
     */
    void setPrimitiveType(GLenum newPrimitiveType) {
        Material::primitiveType = newPrimitiveType;
    }

    /**
     * Returns which side of the mesh will be drawn.
     */
    [[nodiscard]] MaterialSide getSide() const {
        return side;
    }

    /**
     * Set which side to draw.
     * @param newSide   The side(s) which will be drawn (as opposed to being culled)
     */
    void setSide(MaterialSide newSide) {
        Material::side = newSide;
    }

    /**
     * Returns whether or not this material tests for depth while drawing.
     */
    [[nodiscard]] bool isDepthTest() const {
        return depthTest;
    }

    /**
     * Enable or disable depth testing for this material.
     * @param enableDepthTest  Set to true to test for depth while drawing.
     */
    void setDepthTest(bool enableDepthTest) {
        Material::depthTest = enableDepthTest;
    }

    /**
     * Returns whether or not this material writes to depth while drawing.
     */
    [[nodiscard]] bool isWriteDepth() const {
        return writeDepth;
    }

    /**
     * Enable or disable depth writing for this material.
     * @param writeToDepth  Set to true to write to depth while drawing.
     */
    void setWriteDepth(bool writeToDepth) {
        Material::writeDepth = writeToDepth;
    }

private:

    /// The geometry primitive used for drawing.
    GLenum primitiveType = GL_TRIANGLES;

    /// Which face to show (meaning which face(s) won't be culled).
    MaterialSide side = FrontSide;

    /// Whether this material should do depth testing or not.
    bool depthTest;

    /// Whether this material should write to depth or not.
    bool writeDepth;

};


#endif //INC_3D_DEMOS_MATERIAL_H
