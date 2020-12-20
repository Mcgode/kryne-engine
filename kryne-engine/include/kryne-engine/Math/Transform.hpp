/**
 * @file
 * @author Max Godefroy
 * @date 19/12/2020.
 */

#ifndef KRYNE_ENGINE_TRANSFORM_HPP
#define KRYNE_ENGINE_TRANSFORM_HPP

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

using namespace glm;


/**
 * Namespace for handling math code
 */
namespace Math {


    /**
     * Computes a lookAt matrix
     * @param eye       The eye position.
     * @param target    The target position, in the same coordinate space as the eye position.
     * @param up        The up vector, still using the same coordinate space.
     * @return A 3x3 orthogonal transform matrix.
     */
    static mat3 getLookAtMatrix(const vec3 &eye, const vec3 &target, const vec3 up = vec3(0, 1, 0))
    {
        auto z = target - eye;

        if (length(z) == 0.f)
            z.z = 1.f;

        z = normalize(z);
        auto x = cross(up, z);

        if (length(x) == 0.f) {
            if (abs(z.z) == 1.0f)
                z.x += 0.0001f;
            else
                z.z += 0.0001f;

            z = normalize(z);
            x = normalize(cross(up, z));
        } else
            x = normalize(x);

        auto y = cross(z, x);

        return mat3(x, y, z);
    }

};

#endif //KRYNE_ENGINE_TRANSFORM_HPP
