//
// Created by Max Godefroy on 22/11/2020.
//

#include "kryne-engine/Geometry/BoxBufferGeometry.h"

using namespace glm;
using namespace Constants;

BoxBufferGeometry::BoxBufferGeometry(float xSize, float ySize, float zSize): BufferGeometry()
{
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoordinates;
    float x = xSize / 2.f, y = ySize / 2.f, z = zSize / 2.f;

    for (uint32_t i = 0; i < CUBE_AMOUNT_OF_RAW_VERTICES; i ++)
    {
        positions.emplace_back(x * CUBE_VERTEX_POSITIONS[3 * i], y * CUBE_VERTEX_POSITIONS[3 * i + 1], z * CUBE_VERTEX_POSITIONS[3 * i + 2]);
        textureCoordinates.emplace_back(CUBE_VERTEX_UV_COORDINATES[2 * i], CUBE_VERTEX_UV_COORDINATES[2 * i + 1]);
        normals.emplace_back(CUBE_VERTEX_NORMALS[3 * i], CUBE_VERTEX_NORMALS[3 * i + 1], CUBE_VERTEX_NORMALS[3 * i + 2]);
    }

    vector<uint32_t> indexes;
    indexes.reserve(CUBE_AMOUNT_OF_INDEXES);
    for (uint32_t i : CUBE_INDEXES)
        indexes.push_back(i);

    this->setAttribute("position", std::make_unique<BufferAttribute>(flattenVector3(positions), 3));
    this->setAttribute("normal", std::make_unique<BufferAttribute>(flattenVector3(normals), 3));
    this->setAttribute("uv", std::make_unique<BufferAttribute>(flattenVector2(textureCoordinates), 2));

    this->setIndices(indexes);

    this->computeTangents();
}
