//
// Created by max on 22/04/19.
//

#include "common/GeometricShapes.h"

const double PI  = 3.141592653589793238463;
const float  PI_F= 3.14159265358979f;


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

            double phi = ((double) j / n_lat - 0.5) * PI;
            double phi_p = ((double) (j + 1) / n_lat - 0.5) * PI;

            float cp1 = std::cos(phi);
            float sp1 = std::sin(phi);
            float cp2 = std::cos(phi_p);
            float sp2 = std::sin(phi_p);

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
                tangents->push_back(glm::vec3(st2, 0, ct2));
                tangents->push_back(glm::vec3(st1, 0, ct1));
                tangents->push_back(glm::vec3(st1, 0, ct1));
            }

            if (j != 0) {

                if (positions != nullptr) {
                    positions->push_back(glm::vec3(ray * ct2 * cp1, ray * sp1, ray * st2 * cp1));
                    positions->push_back(glm::vec3(ray * ct1 * cp1, ray * sp1, ray * st1 * cp1));
                    positions->push_back(glm::vec3(ray * ct2 * cp2, ray * sp2, ray * st2 * cp2));
                }

                if (normals != nullptr) {
                    normals->push_back(glm::vec3(ct2 * cp1, sp1, st2 * cp1));
                    normals->push_back(glm::vec3(ct1 * cp1, sp1, st1 * cp1));
                    normals->push_back(glm::vec3(ct2 * cp2, sp2, st2 * cp2));
                }

                if (textureCoordinates != nullptr) {
                    textureCoordinates->push_back(glm::vec2(theta_p / (2 * PI), phi / PI + 0.5));
                    textureCoordinates->push_back(glm::vec2(theta / (2 * PI), phi / PI + 0.5));
                    textureCoordinates->push_back(glm::vec2(theta_p / (2 * PI), phi_p / PI + 0.5));
                }

                if (tangents != nullptr) {
                    tangents->push_back(glm::vec3(st2, 0, ct2));
                    tangents->push_back(glm::vec3(st1, 0, ct1));
                    tangents->push_back(glm::vec3(st2, 0, ct2));
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
    float s = size / 2.0;

    if (positions != nullptr) {
        positions->push_back(glm::vec3(-s, 0 , -s));
        positions->push_back(glm::vec3( s, 0 , -s));
        positions->push_back(glm::vec3(-s, 0 ,  s));
        positions->push_back(glm::vec3( s, 0 ,  s));
        positions->push_back(glm::vec3(-s, 0 ,  s));
        positions->push_back(glm::vec3( s, 0 , -s));
    }

    if (normals != nullptr) {
        normals->push_back(glm::vec3(0, 1, 0));
        normals->push_back(glm::vec3(0, 1, 0));
        normals->push_back(glm::vec3(0, 1, 0));
        normals->push_back(glm::vec3(0, 1, 0));
        normals->push_back(glm::vec3(0, 1, 0));
        normals->push_back(glm::vec3(0, 1, 0));
    }

    if (textureCoordinates != nullptr) {
        textureCoordinates->push_back(glm::vec3(0, 0 , 0));
        textureCoordinates->push_back(glm::vec3(1, 0 , 0));
        textureCoordinates->push_back(glm::vec3(0, 0 , 1));
        textureCoordinates->push_back(glm::vec3(1, 0 , 1));
        textureCoordinates->push_back(glm::vec3(0, 0 , 1));
        textureCoordinates->push_back(glm::vec3(1, 0 , 0));
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
