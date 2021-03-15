/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#include "PlaneBufferGeometry.h"

PlaneBufferGeometry::PlaneBufferGeometry(float width, float height, uint32_t widthSegments, uint32_t heightSegments) :
    BufferGeometry()
{
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> uvs;

    vector<uint32_t> indices;

    float halfWidth  = width * 0.5f,
          halfHeight = height * 0.5f;

    for (uint32_t x = 0; x <= widthSegments; x++)
    {
        float dx = (float) x / widthSegments;

        for (uint32_t y = 0; y <= widthSegments; y++)
        {
            float dy = (float) y / widthSegments;

            positions.emplace_back(halfWidth * (dx - 0.5f), halfHeight * (0.5f - dy), 0.f);
            normals.emplace_back(0, 0, 1);
            uvs.emplace_back(dx, dy);
        }
    }

    for (uint32_t x = 0; x < widthSegments; x++)
    {
        uint32_t dx0 = x * (heightSegments + 1);
        uint32_t dx1 = dx0 + heightSegments + 1;

        for (uint32_t y = 0; y < widthSegments; y++)
        {
            indices.push_back(dx0 + y);
            indices.push_back(dx0 + y + 1);
            indices.push_back(dx1 + y + 1);
            indices.push_back(dx0 + y);
            indices.push_back(dx1 + y + 1);
            indices.push_back(dx1 + y);
        }
    }

    this->setAttribute("position", make_unique<BufferAttribute>(Utils::flatMapFloatVectors(positions), 3));
    this->setAttribute("normal", make_unique<BufferAttribute>(Utils::flatMapFloatVectors(normals), 3));
    this->setAttribute("uv", make_unique<BufferAttribute>(Utils::flatMapFloatVectors(uvs), 2));

    this->setIndices(indices);
}
