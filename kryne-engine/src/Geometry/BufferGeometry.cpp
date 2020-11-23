//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/Geometry/BufferGeometry.h"

#include <utility>

BufferGeometry::BufferGeometry()
{
    glGenVertexArrays(1, &this->vao);
}


void BufferGeometry::addAttribute(const string &name, unique_ptr<BufferAttribute> attribute)
{
    GLuint location = this->nextLocation++;
    names.push_back(name);

    attribute->bindToVAO(this->vao, location);
    if (ebo == 0)
        this->length = (location == 0) ? attribute->getLength() : std::min(this->length, attribute->getLength());

    bufferAttributes.push_back(std::move(attribute));
}


void BufferGeometry::draw(GLenum geometry) const
{
    glBindVertexArray(this->vao);
    if (this->ebo) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
        glDrawElements(geometry, length, GL_UNSIGNED_INT, nullptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    } else
        glDrawArrays(geometry, 0, length);
    glBindVertexArray(0);
}


void BufferGeometry::setIndices(vector<uint32_t> newIndexes)
{
    this->indexes = std::move(newIndexes);
    if (this->ebo == 0)
        glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexes.size() * sizeof(uint32_t), this->indexes.data(), GL_STATIC_DRAW); //< TODO : handle non-static draw case
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    auto data = this->indexes.data();

    this->length = this->indexes.size();
}


BufferGeometry::~BufferGeometry()
{
    glDeleteVertexArrays(1, &this->vao);

    if (this->ebo)
        glDeleteBuffers(1, &this->ebo);
}


using namespace glm;
vec3 BufferGeometry::computeTangent(vec3 p0, vec3 p1, vec3 p2, vec2 uv0, vec2 uv1, vec2 uv2) {
    vec3 q0 = p1 - p0;
    vec3 q1 = p2 - p0;

    vec2 deltaUV0 = uv1 - uv0;
    vec2 deltaUV1 = uv2 - uv0;

    float f = 1.f / (deltaUV0.x * deltaUV1.y - deltaUV0.y * deltaUV1.x);

    vec3 T = vec3(
            f * (deltaUV1.y * q0.x - deltaUV0.y * q1.x),
            f * (deltaUV1.y * q0.y - deltaUV0.y * q1.y),
            f * (deltaUV1.y * q0.z - deltaUV0.y * q1.z)
    );

    return normalize(T);
}
