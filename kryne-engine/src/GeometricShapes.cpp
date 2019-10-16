//
// Created by max on 22/04/19.
//

#include <glm/glm.hpp>
#include "common/GeometricShapes.h"

const double PI  = 3.141592653589793238463;
const float  PI_F= 3.14159265358979f;


vec3 computeTangent(vec3 p0, vec3 p1, vec3 p2, vec2 uv0, vec2 uv1, vec2 uv2) {
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


void generateSphereShape(
        float ray,
        int n_long,
        int n_lat,
        std::vector<vec3> *positions,
        std::vector<vec3> *normals,
        std::vector<vec2> *textureCoordinates,
        std::vector<vec3> *tangents)
{
    for (int i = 0; i < n_long; i++) {

        double theta = i * 2 * PI / n_long;
        double theta_p = (i + 1) * 2 * PI / n_long;

        float ct1 = std::cos(theta);
        float st1 = std::sin(theta);
        float ct2 = std::cos(theta_p);
        float st2 = std::sin(theta_p);

        for (int j = 0; j < n_lat; j++) {

            glm::vec3 p0, p1, p2;
            glm::vec2 uv0, uv1, uv2;

            double phi = ((double) j / n_lat - 0.5) * PI;
            double phi_p = ((double) (j + 1) / n_lat - 0.5) * PI;

            float cp1 = std::cos(phi);
            float sp1 = std::sin(phi);
            float cp2 = std::cos(phi_p);
            float sp2 = std::sin(phi_p);

            p0 = glm::vec3(ray * ct2 * cp2, ray * sp2, ray * st2 * cp2);
            p1 = glm::vec3(ray * ct1 * cp1, ray * sp1, ray * st1 * cp1);
            p2 = glm::vec3(ray * ct1 * cp2, ray * sp2, ray * st1 * cp2);

            uv0 = glm::vec2(theta_p / (2 * PI), phi_p / PI + 0.5);
            uv1 = glm::vec2(theta   / (2 * PI), phi   / PI + 0.5);
            uv2 = glm::vec2(theta   / (2 * PI), phi_p / PI + 0.5);

            if (positions != nullptr) {
                positions->push_back(glm::vec3(ray * ct2 * cp2, ray * sp2, ray * st2 * cp2));
                positions->push_back(glm::vec3(ray * ct1 * cp1, ray * sp1, ray * st1 * cp1));
                positions->push_back(glm::vec3(ray * ct1 * cp2, ray * sp2, ray * st1 * cp2));
            }

            if (normals != nullptr) {
                normals->push_back(glm::vec3(ct2 * cp2, sp2, st2 * cp2));
                normals->push_back(glm::vec3(ct1 * cp1, sp1, st1 * cp1));
                normals->push_back(glm::vec3(ct1 * cp2, sp2, st1 * cp2));
            }

            if (textureCoordinates != nullptr) {
                textureCoordinates->push_back(glm::vec2(theta_p / (2 * PI), phi_p / PI + 0.5));
                textureCoordinates->push_back(glm::vec2(theta   / (2 * PI), phi   / PI + 0.5));
                textureCoordinates->push_back(glm::vec2(theta   / (2 * PI), phi_p / PI + 0.5));
            }

            if (tangents != nullptr) {
                glm::vec3 q0 = p1 - p0;
                glm::vec3 q1 = p2 - p0;

                glm::vec2 deltaUV0 = uv1 - uv0;
                glm::vec2 deltaUV1 = uv2 - uv0;

                float f = 1.f / (deltaUV0.x * deltaUV1.y - deltaUV0.y * deltaUV1.x);

                glm::vec3 T = glm::vec3(
                        f * (deltaUV1.y * q0.x - deltaUV0.y * q1.x),
                        f * (deltaUV1.y * q0.y - deltaUV0.y * q1.y),
                        f * (deltaUV1.y * q0.z - deltaUV0.y * q1.z)
                );
                T = glm::normalize(T);

                tangents->push_back(T);
                tangents->push_back(T);
                tangents->push_back(T);
            }

            if (j != 0) {

                p0 = glm::vec3(ray * ct2 * cp1, ray * sp1, ray * st2 * cp1);
                p1 = glm::vec3(ray * ct1 * cp1, ray * sp1, ray * st1 * cp1);
                p2 = glm::vec3(ray * ct2 * cp2, ray * sp2, ray * st2 * cp2);

                uv0 = glm::vec2(theta_p / (2 * PI), phi / PI + 0.5);
                uv1 = glm::vec2(theta / (2 * PI), phi / PI + 0.5);
                uv2 = glm::vec2(theta_p / (2 * PI), phi_p / PI + 0.5);

                if (positions != nullptr) {
                    positions->push_back(p0);
                    positions->push_back(p1);
                    positions->push_back(p2);
                }

                if (normals != nullptr) {
                    normals->push_back(glm::vec3(ct2 * cp1, sp1, st2 * cp1));
                    normals->push_back(glm::vec3(ct1 * cp1, sp1, st1 * cp1));
                    normals->push_back(glm::vec3(ct2 * cp2, sp2, st2 * cp2));
                }

                if (textureCoordinates != nullptr) {
                    textureCoordinates->push_back(uv0);
                    textureCoordinates->push_back(uv1);
                    textureCoordinates->push_back(uv2);
                }

                if (tangents != nullptr) {
                    vec3 T = computeTangent(p0, p1, p2, uv0, uv1, uv2);

                    tangents->push_back(T);
                    tangents->push_back(T);
                    tangents->push_back(T);
                }

            }

        }
    }
}


void generateQuadShape(float size,
                       std::vector<vec3> *positions,
                       std::vector<vec3> *normals,
                       std::vector<vec2> *textureCoordinates,
                       std::vector<vec3> *tangents)
{
    float s = size / 2.0f;

    if (positions != nullptr) {
        positions->push_back(glm::vec3(-s, -s, 0));
        positions->push_back(glm::vec3( s, -s, 0));
        positions->push_back(glm::vec3(-s,  s, 0));
        positions->push_back(glm::vec3( s,  s, 0));
        positions->push_back(glm::vec3(-s,  s, 0));
        positions->push_back(glm::vec3( s, -s, 0));
    }

    if (normals != nullptr) {
        normals->push_back(glm::vec3(0, 0, 1));
        normals->push_back(glm::vec3(0, 0, 1));
        normals->push_back(glm::vec3(0, 0, 1));
        normals->push_back(glm::vec3(0, 0, 1));
        normals->push_back(glm::vec3(0, 0, 1));
        normals->push_back(glm::vec3(0, 0, 1));
    }

    if (textureCoordinates != nullptr) {
        textureCoordinates->push_back(glm::vec2(0, 0));
        textureCoordinates->push_back(glm::vec2(1, 0));
        textureCoordinates->push_back(glm::vec2(0, 1));
        textureCoordinates->push_back(glm::vec2(1, 1));
        textureCoordinates->push_back(glm::vec2(0, 1));
        textureCoordinates->push_back(glm::vec2(1, 0));
    }

    if (tangents != nullptr) {
        tangents->push_back(glm::vec3(1, 0, 0));
        tangents->push_back(glm::vec3(1, 0, 0));
        tangents->push_back(glm::vec3(1, 0, 0));
        tangents->push_back(glm::vec3(1, 0, 0));
        tangents->push_back(glm::vec3(1, 0, 0));
        tangents->push_back(glm::vec3(1, 0, 0));
    }
}


void generateDoubleSidedQuadShape(float size, std::vector<vec3> *positions, std::vector<vec3> *normals,
                                  std::vector<vec2> *textureCoordinates, std::vector<vec3> *tangents)
{
    generateQuadShape(size, positions, normals, textureCoordinates, tangents);
    float s = size / 2.0f;

    if (positions != nullptr) {
        positions->push_back(glm::vec3(-s, -s, 0));
        positions->push_back(glm::vec3(-s,  s, 0));
        positions->push_back(glm::vec3( s, -s, 0));
        positions->push_back(glm::vec3( s,  s, 0));
        positions->push_back(glm::vec3( s, -s, 0));
        positions->push_back(glm::vec3(-s,  s, 0));
    }

    if (normals != nullptr) {
        normals->push_back(glm::vec3(0, 0, -1));
        normals->push_back(glm::vec3(0, 0, -1));
        normals->push_back(glm::vec3(0, 0, -1));
        normals->push_back(glm::vec3(0, 0, -1));
        normals->push_back(glm::vec3(0, 0, -1));
        normals->push_back(glm::vec3(0, 0, -1));
    }

    if (textureCoordinates != nullptr) {
        textureCoordinates->push_back(glm::vec2(0, 0));
        textureCoordinates->push_back(glm::vec2(0, 1));
        textureCoordinates->push_back(glm::vec2(1, 0));
        textureCoordinates->push_back(glm::vec2(1, 1));
        textureCoordinates->push_back(glm::vec2(1, 0));
        textureCoordinates->push_back(glm::vec2(0, 1));
    }

    if (tangents != nullptr) {
        tangents->push_back(glm::vec3(-1, 0, 0));
        tangents->push_back(glm::vec3(-1, 0, 0));
        tangents->push_back(glm::vec3(-1, 0, 0));
        tangents->push_back(glm::vec3(-1, 0, 0));
        tangents->push_back(glm::vec3(-1, 0, 0));
        tangents->push_back(glm::vec3(-1, 0, 0));
    }
}


void generateCubeShape(float xSize, float ySize, float zSize, std::vector<vec3> *positions, std::vector<vec3> *normals,
                       std::vector<vec2> *textureCoordinates, std::vector<vec3> *tangents)
{
    float x = xSize / 2.f, y = ySize / 2.f, z = zSize / 2.f;

    for (int i = 0; i < CUBE_AMOUNT_OF_VERTICES; i += 3)
    {
        int j = i;
        vec3 p0 = vec3(x*CUBE_VERTEX_POSITIONS[3*j], y*CUBE_VERTEX_POSITIONS[3*j+1], z*CUBE_VERTEX_POSITIONS[3*j+2]);
        vec2 uv0 = vec2(CUBE_VERTEX_UV_COORDINATES[2*j], CUBE_VERTEX_UV_COORDINATES[2*j+1]);
        if (normals != nullptr)
            normals->push_back(vec3(CUBE_VERTEX_NORMALS[3*j], CUBE_VERTEX_NORMALS[3*j+1], CUBE_VERTEX_NORMALS[3*j+2]));

        j++;
        vec3 p1 = vec3(x*CUBE_VERTEX_POSITIONS[3*j], y*CUBE_VERTEX_POSITIONS[3*j+1], z*CUBE_VERTEX_POSITIONS[3*j+2]);
        vec2 uv1 = vec2(CUBE_VERTEX_UV_COORDINATES[2*j], CUBE_VERTEX_UV_COORDINATES[2*j+1]);
        if (normals)
            normals->push_back(vec3(CUBE_VERTEX_NORMALS[3*j], CUBE_VERTEX_NORMALS[3*j+1], CUBE_VERTEX_NORMALS[3*j+2]));

        j++;
        vec3 p2 = vec3(x*CUBE_VERTEX_POSITIONS[3*j], y*CUBE_VERTEX_POSITIONS[3*j+1], z*CUBE_VERTEX_POSITIONS[3*j+2]);
        vec2 uv2 = vec2(CUBE_VERTEX_UV_COORDINATES[2*j], CUBE_VERTEX_UV_COORDINATES[2*j+1]);
        if (normals != nullptr)
            normals->push_back(vec3(CUBE_VERTEX_NORMALS[3*j], CUBE_VERTEX_NORMALS[3*j+1], CUBE_VERTEX_NORMALS[3*j+2]));

        if (positions) {
            positions->push_back(p0);
            positions->push_back(p1);
            positions->push_back(p2);
        }

        if (textureCoordinates) {
            textureCoordinates->push_back(uv0);
            textureCoordinates->push_back(uv1);
            textureCoordinates->push_back(uv2);
        }

        if (tangents) {
            vec3 T = computeTangent(p0, p1, p2, uv0, uv1, uv2);
            tangents->push_back(T);
            tangents->push_back(T);
            tangents->push_back(T);
        }
    }
}
