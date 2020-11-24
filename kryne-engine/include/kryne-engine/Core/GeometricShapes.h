/**
 * @file
 * @author Max Godefroy
 * @date 22/04/19
 */

#ifndef INC_3D_DEMOS_GEOMETRICSHAPES_H
#define INC_3D_DEMOS_GEOMETRICSHAPES_H

#include <vector>
#include <cmath>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <kryne-engine/Constants/CubeVertexData.h>


using namespace glm;


/**
 * Generate a single sided quad shape
 * @param size                  The length of each side of the quad
 * @param positions             The vector for collecting the vertex positions
 * @param normals               The vector for collecting the vertex normals
 * @param textureCoordinates    The vector for collecting the UV map coordinates for the vertex
 * @param tangents              The vector for collecting the vertex tangents
 */
void generateQuadShape(
        float size = 1.0f,
        std::vector<vec3> *positions = nullptr,
        std::vector<vec3> *normals = nullptr,
        std::vector<vec2> *textureCoordinates = nullptr,
        std::vector<vec3> *tangents = nullptr);


/**
 * Generates the 3D data of a sphere shape
 * @param ray                   The ray of the sphere
 * @param n_long                Number of discrete longitudes
 * @param n_lat                 Number of discrete latitudes
 * @param positions             The vector for collecting the vertex positions
 * @param normals               The vector for collecting the vertex normals
 * @param textureCoordinates    The vector for collecting the UV map coordinates for the vertex
 * @param tangents              The vector for collecting the vertex tangents
 */
void generateSphereShape(
        float ray,
        int n_long = 100,
        int n_lat = 50,
        std::vector<vec3> *positions = nullptr,
        std::vector<vec3> *normals = nullptr,
        std::vector<vec2> *textureCoordinates = nullptr,
        std::vector<vec3> *tangents = nullptr);

/**
 * Generate a double sided quad shape
 * @param size                  The length of each side of the quad
 * @param positions             The vector for collecting the vertex positions
 * @param normals               The vector for collecting the vertex normals
 * @param textureCoordinates    The vector for collecting the UV map coordinates for the vertex
 * @param tangents              The vector for collecting the vertex tangents
 */
void generateDoubleSidedQuadShape(
        float size = 1.0f,
        std::vector<vec3> *positions = nullptr,
        std::vector<vec3> *normals = nullptr,
        std::vector<vec2> *textureCoordinates = nullptr,
        std::vector<vec3> *tangents = nullptr);


/**
 * Generate a cube shape
 * @param xSize                 The x width of the cube
 * @param ySize                 The y width of the cube
 * @param zSize                 The z width of the cube
 * @param positions             The vector for collecting the vertex positions
 * @param normals               The vector for collecting the vertex normals
 * @param textureCoordinates    The vector for collecting the UV map coordinates for the vertex
 * @param tangents              The vector for collecting the vertex tangents
 */
void generateCubeShape(
        float xSize = 1.0f,
        float ySize = 1.0f,
        float zSize = 1.0f,
        std::vector<vec3> *positions = nullptr,
        std::vector<vec3> *normals = nullptr,
        std::vector<vec2> *textureCoordinates = nullptr,
        std::vector<vec3> *tangents = nullptr);

#endif //INC_3D_DEMOS_GEOMETRICSHAPES_H
