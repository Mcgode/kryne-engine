/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#pragma once


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/norm.hpp>


using namespace glm;


namespace Math {

    struct Plane {

        vec3 normal;

        float d;

        void setCoefficients(vec4 coefficients)
        {
            normal = vec3(coefficients);
            float l = length(normal);
            normal /= l;
            d = coefficients.w / l;
        }

        [[nodiscard]] inline float distance(const vec3 &point) const
        {
            return d + dot(normal, point);
        }

    };

}