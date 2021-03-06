/**
 * @file
 * @author Max Godefroy
 * @date 21/11/2020
 */

#ifndef INC_KRYNE_ENGINE_BUFFERATTRIBUTE_H
#define INC_KRYNE_ENGINE_BUFFERATTRIBUTE_H

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
     * @param usage  The draw type for this buffer attribute.
     */
    BufferAttribute(vector<float> data, uint32_t itemSize, GLint usage = GL_STATIC_DRAW);

    /**
     * Updates the VBO and uploads its data to the GPU
     */
    void updateData();

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
     * Returns the item size for this buffer
     */
    [[nodiscard]] uint32_t getItemSize() const { return itemSize; }

    /**
     * Returns the data as a pointer array
     */
    [[nodiscard]] const float *getData() const { return data.data(); }

    /**
     * Returns whether or not the attribute needs to be updated
     */
    inline bool needsUpdate() const { return this->needUpdate; }

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

    /// The VBO usage tag.
    GLuint usage;

    /// If true, data needs to be updated and uploaded.
    bool needUpdate;


public:

    /**
     * Infers the type string for this attribute. Used for automatic shader attribute layout code generation.
     * @return If the type couldn't be inferred, the string will be empty.
     */
    [[nodiscard]] string inferTypeString() const;

};


#endif //INC_KRYNE_ENGINE_BUFFERATTRIBUTE_H
