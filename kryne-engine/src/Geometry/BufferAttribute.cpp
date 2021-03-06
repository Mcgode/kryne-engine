//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/Geometry/BufferAttribute.h"

BufferAttribute::BufferAttribute(vector<float> data, uint32_t itemSize, GLint usage) :
        data(data),
        itemSize(itemSize),
        usage(usage),
        needUpdate(true)
{}


void BufferAttribute::updateData()
{
    if (this->vbo == 0)
        glGenBuffers(1, &this->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), this->usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->needUpdate = false;
}


void BufferAttribute::bindToVAO(GLuint vao, GLuint location) const
{
    if (this->vbo == 0)
        return;

    glBindVertexArray(vao);

    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(location, this->itemSize, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}


BufferAttribute::~BufferAttribute()
{
    if (this->vbo != 0)
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
