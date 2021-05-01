/**
 * @file
 * @author Max Godefroy
 * @date 08/04/2021.
 */

#include "kryne-engine/Geometry/TorusKnotBufferGeometry.hpp"
#include <kryne-engine/Utils/Utils.h>


using namespace glm;


TorusKnotBufferGeometry::TorusKnotBufferGeometry(float radius,
                                                 float tube,
                                                 uint32_t tubularSegments,
                                                 uint32_t radialSegments,
                                                 uint32_t p,
                                                 uint32_t q)
{
    const auto PI = glm::pi<float>();

    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> uvs;

    for (uint32_t i = 0; i <= tubularSegments; i++)
    {
        const float u = (float) i / tubularSegments * p * PI * 2;

        auto p1 = TorusKnotBufferGeometry::computePositionOnCurve(u, p, q, radius);
        auto p2 = TorusKnotBufferGeometry::computePositionOnCurve(u + .01f, p, q, radius);

        auto T = p2 - p1;
        auto N = p2 + p1;
        auto B = cross(T, N);
        N = normalize(cross(B, T));
        B = normalize(B);

        for (uint32_t j = 0; j <= radialSegments; j++)
        {
            const float v = 2.f * j * PI / radialSegments,
                        cx = -tube * cos(v),
                        cy =  tube * sin(v);

            vec3 vertex(p1.x + (cx * N.x + cy * B.x),
                        p1.y + (cx * N.y + cy * B.y),
                        p1.z + (cx * N.z + cy * B.z));
            positions.push_back(vertex);

            normals.push_back(normalize(vertex - p1));

            uvs.emplace_back((float) i / tubularSegments, (float) j / radialSegments);
        }
    }

    vector<uint32_t> indices;
    for (uint32_t j = 1; j <= tubularSegments; j++)
    {
        for (uint32_t i = 1; i <= radialSegments; i++)
        {
            const uint32_t a = ( radialSegments + 1 ) * ( j - 1 ) + ( i - 1 ),
                           b = ( radialSegments + 1 ) * j + ( i - 1 ),
                           c = ( radialSegments + 1 ) * j + i,
                           d = ( radialSegments + 1 ) * ( j - 1 ) + i;

            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    using Utils::flatMapFloatVectors;
    this->setAttribute("position", std::make_unique<BufferAttribute>(flatMapFloatVectors(positions), 3));
    this->setAttribute("normal", std::make_unique<BufferAttribute>(flatMapFloatVectors(normals), 3));
    this->setAttribute("uv", std::make_unique<BufferAttribute>(flatMapFloatVectors(uvs), 2));

    this->setIndices(indices);
}

vec3 TorusKnotBufferGeometry::computePositionOnCurve(float u, uint32_t p, uint32_t q, float radius)
{
    const float cu = cos(u),
                su = sin(u),
                quOverP = (float) q / p * u,
                cs = cos(quOverP);

    return vec3(radius * (2.f + cs) * .5f * cu,
                radius * (2.f + cs) * .5f * su,
                radius * sin(quOverP) * .5f);
}
