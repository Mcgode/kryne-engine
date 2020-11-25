/**
 * @file
 * @author Max Godefroy
 * @date 21/11/2020
 */

#ifndef INC_3D_DEMOS_BUFFERATTRIBUTE_H
#define INC_3D_DEMOS_BUFFERATTRIBUTE_H

#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

/**
 * Handles a geometry buffer attribute.
 */
class BufferAttribute {

public:

    /**
     * Initializes a new buffer attribute.
     * @param data      The data for this attribute, as a vector of floats.
     * @param itemSize  The number of floats per item.
     * @param drawType  The draw type for this buffer attribute.
     */
    BufferAttribute(vector<float> data, uint32_t itemSize, GLint drawType = GL_STATIC_DRAW);

    /**
     * Binds the buffer to a vertex array object.
     * @param vao       The VAO to bind the buffer to.
     * @param location  The buffer location to bind to.
     */
    void bindToVAO(GLuint vao, GLuint location) const;

    /**
     * Retrieves the length of the data, in number of items
     */
    uint32_t getLength();

    /**
     * Returns the data as a pointer array
     */
    [[nodiscard]] const float *getData() const { return data.data(); }

    /**
     * Deletes the buffer and clears its memory
     */
    ~BufferAttribute();

private:

    /// The buffer data
    vector<float> data;

    /// The size of each item, in number of floats
    uint32_t itemSize;

    /// The VBO id of the buffer
    GLuint vbo {};

};


#endif //INC_3D_DEMOS_BUFFERATTRIBUTE_H
