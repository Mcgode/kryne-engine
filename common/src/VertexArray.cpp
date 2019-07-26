//
// Created by max on 19/04/19.
//

#include "common/VertexArray.h"


VertexArray::VertexArray(std::vector<glm::vec2> *first_data, GLenum usage)
{
    vao = 0, amount_of_locations = 0, ebo = 0, ebo_length = 0;
    this->usage = usage;
    glGenVertexArrays(1, &vao);
    length = first_data->size();
    this->add_array(first_data);
}


VertexArray::VertexArray(std::vector<glm::vec3> *first_data, GLenum usage)
{
    vao = 0, amount_of_locations = 0, ebo = 0, ebo_length = 0;
    this->usage = usage;
    glGenVertexArrays(1, &vao);
    length = first_data->size();
    this->add_array(first_data);
}


void VertexArray::add_array(std::vector<glm::vec2> *data)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    assert(data->size() == length);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(amount_of_locations);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(glm::vec2), data->data(), usage);
    glVertexAttribPointer(amount_of_locations, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    amount_of_locations++;
    vbos.push_back(vbo);
}


void VertexArray::add_array(std::vector<glm::vec3> *data)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    assert(data->size() == length);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(amount_of_locations);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(glm::vec3), data->data(), usage);
    glVertexAttribPointer(amount_of_locations, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    amount_of_locations++;
    vbos.push_back(vbo);
}


VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vao);
//    for (GLuint vbo: vbos)
//        glDeleteBuffers(1, &vbo);
    glDeleteBuffers(vbos.size(), vbos.data());
    if (ebo != 0)
        glDeleteBuffers(1, &ebo);
}


void VertexArray::execute(GLenum primitive)
{
    glBindVertexArray(vao);
    if (ebo != 0)
        glDrawElements(primitive, ebo_length, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(primitive, 0, length);
    glBindVertexArray(0);
}


void VertexArray::setUpElements(std::vector<uint32_t[3]> *indexes, size_t size)
{
    glGenBuffers(1, &ebo);
    ebo_length = indexes->size();

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes->size() * size * sizeof(uint32_t), indexes->data(), usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


