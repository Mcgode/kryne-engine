//
// Created by max on 22/04/19.
//

#ifndef INC_3D_DEMOS_BASEOBJECT_H
#define INC_3D_DEMOS_BASEOBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

#include "Shader.h"
#include "VertexArray.h"
#include "AdditionalParameters.h"


/**
 * The base 3D object
 */
class BaseObject {

public:

    /**
     * The base 3D object
     * @param shader        The shader for this object
     * @param vertexArray   The vertex array containing the 3D data for this object
     */
    explicit BaseObject(Shader *shader = nullptr, VertexArray *vertexArray = nullptr);

    /**
     * Sets the shader of the object
     * @param shader The shader for the object
     */
    void setShader(Shader *shader);

    /**
     * A function for drawing the object
     * @param projection    The projection transform matrix
     * @param view          The view transform matrix
     * @param model         The model transform matrix
     * @param params        A map containing additional data to pass down to the object
     */
    void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model, AdditionalParameters *params);

    Shader *getShader() const;

    bool isSupportingLighting() const;

private:

    Shader *shader{};

    VertexArray *vertexArray{};

    bool supportsLighting{};

    int maximumSupportedPointLights{};

    bool castsHardShadows{};

};


#endif //INC_3D_DEMOS_BASEOBJECT_H
