/**
 * @file
 * @author Max Godefroy
 * @date 21/11/2020
 */

#ifndef INC_KRYNE_ENGINE_BUFFERGEOMETRY_H
#define INC_KRYNE_ENGINE_BUFFERGEOMETRY_H

#include <algorithm>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <functional>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <kryne-engine/Dispatch/Dispatcher.h>

#include "BufferAttribute.h"

using namespace std;


/**
 * Handles a geometry that will be rendered to the screen.
 */
class BufferGeometry {

public:
    /**
     * Initializes a buffer geometry.
     */
    BufferGeometry();

    /**
     * Sets the value of a buffer attribute.
     * Will update a previous attribute or override it if needs be.
     * Will update BufferGeometry::length if needed.
     * Will update the shader attribute layout code for this geometry, by calling BufferGeometry::updateLayoutCode.
     *
     * @param name      The name for this attribute.
     * @param attribute The BufferAttribute object itself.
     */
    void setAttribute(const string &name, unique_ptr<BufferAttribute> attribute);

    /**
     * Draws the geometry.
     * @param geometry  The OpenGL primitive to use for drawing the geometry.
     */
    void draw(GLenum geometry = GL_TRIANGLES) const;

    /**
     * Set the vertex indexes for the EBO.
     * @param newIndexes    The indexes for the faces of the geometry.
     */
    void setIndices(vector<uint32_t> newIndexes);

    /**
     * Deletes the geometry, and frees the VAO.
     */
    virtual ~BufferGeometry();

protected:

    /**
     * Updates the total vertex length for this geometry.
     */
    void updateLength();

protected:

    /// The VAO id.
    GLuint vao {};

    /// The location for the next buffer attribute.
    GLuint nextLocation = 0;

    /// The attributes of the geometry, stored by name, containing both the BufferAttribute and its location.
    unordered_map<string, pair<unique_ptr<BufferAttribute>, GLint>> attributes;

    /// The length of the vertices for this geometry. Used for drawing.
    uint32_t length = 0;

    /// The indexes for the geometry faces.
    vector<uint32_t> indexes;

    /// The id of the EBO if one is used.
    GLuint ebo = 0;

public:

    /**
     * Computes the 'tangent' attribute for this geometry.
     * Requires both the 'position' and 'uv' attributes to be set.
     * @returns `true` if successful at computing the tangents, `false` otherwise.
     */
    bool computeTangents();

protected:

    /**
     * Computes the tangent of a given face
     * @param p0    The first vertex position
     * @param p1    The second vertex position
     * @param p2    The third vertex position
     * @param uv0   The first vertex uv coordinates
     * @param uv1   The second vertex uv coordinates
     * @param uv2   The third vertex uv coordinates
     * @return The tangent for this face
     */
    static glm::vec3 computeTangent(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2);


public:

    /**
     * Returns the vertex shader layout code, based off the layout data of this geometry.
     * @returns A pair containing the code as a string, and its hash (for accelerated string comparison)
     */
    [[nodiscard]] const pair<string, uint64_t> &getShaderLayoutCode() const { return BufferGeometry::layoutCode; };

protected:

    /**
     * Updates the shader layout code.
     */
    void updateLayoutCode();

protected:

    /// The shader layout code along its hash value.
    pair<string, uint64_t> layoutCode {};

};


#endif //INC_KRYNE_ENGINE_BUFFERGEOMETRY_H
