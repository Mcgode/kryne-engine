//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/Geometry/BufferAttribute.h"

BufferAttribute::BufferAttribute(vector<float> data, uint32_t itemSize, GLint drawType) :
    data(data),
    itemSize(itemSize)
{
    glGenBuffers(1, &this->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), drawType);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void BufferAttribute::bindToVAO(GLuint vao, GLuint location) const
{
    glBindVertexArray(vao);

    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(location, this->itemSize, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}


BufferAttribute::~BufferAttribute()
{
    glDeleteBuffers(1, &this->vbo);
}


uint32_t BufferAttribute::getLength()
{
    return this->data.size() / this->itemSize;
}


string BufferAttribute::inferTypeString() const
{
    switch (itemSize) {
        case 1:
            return "float";
        case 2:
            return "vec2";
        case 3:
            return "vec3";
        case 4:
            return "vec4";
        case 9:
            return "mat3";
        case 16:
            return "mat4";
        default:
            return string();
    }
}
