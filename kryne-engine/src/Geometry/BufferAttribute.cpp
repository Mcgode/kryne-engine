//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/Geometry/BufferAttribute.h"

template<class T>
BufferAttribute<T>::BufferAttribute(vector<T> data, uint32_t itemSize, GLint drawType) :
    data(data),
    itemSize(itemSize)
{
    glGenBuffers(1, &this->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), drawType);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


template<class T>
void BufferAttribute<T>::bindToVAO(GLuint vao, GLuint location)
{
    glBindVertexArray(vao);

    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(location, this->itemSize, GL_FLOAT, GL_FALSE, sizeof(T), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}


template<class T>
BufferAttribute<T>::~BufferAttribute()
{
    glDeleteBuffers(1, &this->vbo);
}


template<class T>
uint32_t BufferAttribute<T>::getLength()
{
    return data.size() * sizeof(T) / sizeof(float) / this->itemSize;
}
