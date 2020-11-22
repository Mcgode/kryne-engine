//
// Created by Max Godefroy on 21/11/2020.
//

#ifndef INC_3D_DEMOS_BUFFERGEOMETRY_H
#define INC_3D_DEMOS_BUFFERGEOMETRY_H

#include <algorithm>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "BufferAttribute.h"

using namespace std;


class BufferGeometry {

public:
    BufferGeometry();

    void addAttribute(const string &name, unique_ptr<BufferAttribute> attribute);

    void draw(GLenum geometry = GL_TRIANGLES) const;

    void setIndices(vector<uint32_t> newIndexes);

    virtual ~BufferGeometry();

protected:
    GLuint vao {};

    GLuint nextLocation = 0;

    vector<string> names;

    vector<unique_ptr<BufferAttribute>> bufferAttributes;

    uint32_t length = 0;

    vector<uint32_t> indexes;

    GLuint ebo = 0;

    static glm::vec3 computeTangent(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2);

};


#endif //INC_3D_DEMOS_BUFFERGEOMETRY_H
