//
// Created by Max Godefroy on 22/11/2020.
//

#include "kryne-engine/Geometry/BoxBufferGeometry.h"

using namespace glm;

BoxBufferGeometry::BoxBufferGeometry(float xSize, float ySize, float zSize): BufferGeometry()
{
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> textureCoordinates;
    vector<vec3> tangents;
    float x = xSize / 2.f, y = ySize / 2.f, z = zSize / 2.f;

    for (int i = 0; i < CUBE_AMOUNT_OF_VERTICES; i += 3)
    {
        int j = i;
        vec3 p0 = vec3(x*CUBE_VERTEX_POSITIONS[3*j], y*CUBE_VERTEX_POSITIONS[3*j+1], z*CUBE_VERTEX_POSITIONS[3*j+2]);
        vec2 uv0 = vec2(CUBE_VERTEX_UV_COORDINATES[2*j], CUBE_VERTEX_UV_COORDINATES[2*j+1]);
        normals.emplace_back(CUBE_VERTEX_NORMALS[3*j], CUBE_VERTEX_NORMALS[3*j+1], CUBE_VERTEX_NORMALS[3*j+2]);

        j++;
        vec3 p1 = vec3(x*CUBE_VERTEX_POSITIONS[3*j], y*CUBE_VERTEX_POSITIONS[3*j+1], z*CUBE_VERTEX_POSITIONS[3*j+2]);
        vec2 uv1 = vec2(CUBE_VERTEX_UV_COORDINATES[2*j], CUBE_VERTEX_UV_COORDINATES[2*j+1]);
        normals.emplace_back(CUBE_VERTEX_NORMALS[3*j], CUBE_VERTEX_NORMALS[3*j+1], CUBE_VERTEX_NORMALS[3*j+2]);

        j++;
        vec3 p2 = vec3(x*CUBE_VERTEX_POSITIONS[3*j], y*CUBE_VERTEX_POSITIONS[3*j+1], z*CUBE_VERTEX_POSITIONS[3*j+2]);
        vec2 uv2 = vec2(CUBE_VERTEX_UV_COORDINATES[2*j], CUBE_VERTEX_UV_COORDINATES[2*j+1]);
        normals.emplace_back(CUBE_VERTEX_NORMALS[3*j], CUBE_VERTEX_NORMALS[3*j+1], CUBE_VERTEX_NORMALS[3*j+2]);

        positions.push_back(p0);
        positions.push_back(p1);
        positions.push_back(p2);

        textureCoordinates.push_back(uv0);
        textureCoordinates.push_back(uv1);
        textureCoordinates.push_back(uv2);

        vec3 T = BufferGeometry::computeTangent(p0, p1, p2, uv0, uv1, uv2);
        tangents.push_back(T);
        tangents.push_back(T);
        tangents.push_back(T);
    }

    vector<uint32_t> indexes;
    indexes.reserve(sizeof(CUBE_INDEXES) / sizeof(uint32_t));
    for (uint32_t i : CUBE_INDEXES)
        indexes.push_back(i);

    this->addAttribute("position", std::make_unique<BufferAttribute>(flattenVector3(positions), 3));
    this->addAttribute("normal",   std::make_unique<BufferAttribute>(flattenVector3(normals), 3));
    this->addAttribute("uv",       std::make_unique<BufferAttribute>(flattenVector2(textureCoordinates), 2));
    this->addAttribute("tangent",  std::make_unique<BufferAttribute>(flattenVector3(tangents), 3));

    this->setIndices(indexes);
}
