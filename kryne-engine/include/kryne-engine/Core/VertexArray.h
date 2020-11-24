/**
 * @file
 * @author Max Godefroy
 * @date 19/04/19
 */

#ifndef INC_3D_DEMOS_VERTEXARRAY_H
#define INC_3D_DEMOS_VERTEXARRAY_H

#include <vector>
#include <stdarg.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


/**
 * A class for handling a vertex array object
 */
class VertexArray {

public:

    /**
     * A class for handling a vertex array object
     * @param first_data    The array of glm::vec2 data for the first location of the vertex array object
     * @param usage         The type of draw for this data
     */
    explicit VertexArray(std::vector<glm::vec2> *first_data, GLenum usage = GL_STATIC_DRAW);

    /**
     * A class for handling a vertex array object
     * @param first_data    The array of glm::vec3 data for the first location of the vertex array object
     * @param usage         The type of draw for this data
     */
    explicit VertexArray(std::vector<glm::vec3> *first_data, GLenum usage = GL_STATIC_DRAW);

    /**
     * Adds some data to the vertex array
     * @param data  The vec2 data to add
     * @param usage The draw type for this data
     */
    void add_array(std::vector<glm::vec2> *data);

    /**
     * Adds some data to the vertex array
     * @param data  The vec3 data to add
     * @param usage The draw type for this data
     */
    void add_array(std::vector<glm::vec3> *data);

    /**
     * Safely deletes all vaos and vbos
     */
    ~VertexArray();

    /**
     * Draws the vertex array to the current framebuffer
     * @param primitive The rendering primitive
     */
    void execute(GLenum primitive = GL_TRIANGLES);

    /**
     * Sets up the element buffer object for the array
     * @param indexes   The indexes of the faces
     * @param size      The number of elements per face
     */
    void setUpElements(std::vector<uint32_t> *indexes, size_t size);

private:

    /// The id of the VAO
    GLuint vao;

    /// The draw usage of the vertex array
    GLint usage;

    /// The ids of the different vbos
    std::vector<GLuint> vbos;

    /// The total length of elements in the VAO
    uint32_t length;

    /// The amount of different data pieces for each vertex
    uint32_t amount_of_locations;

    /// The id of the linked EBO. Set to 0 if no EBO is linked.
    GLuint ebo;

    /// The amount of faces to draw
    uint32_t ebo_length;

};


#endif //INC_3D_DEMOS_VERTEXARRAY_H
