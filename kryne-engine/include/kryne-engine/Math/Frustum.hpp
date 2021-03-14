/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#pragma once


#include "Sphere.hpp"
#include "Plane.hpp"


namespace Math {

    struct Frustum {

        enum Sides {
            Near = 0,
            Far,
            Bottom,
            Top,
            Left,
            Right,
        };

        Plane planes[6] {};

        explicit Frustum(const mat4 &cameraMatrix)
        {
            const mat4 m = transpose(cameraMatrix);

            planes[Near]  .setCoefficients(m[3] + m[2]);
            planes[Far]   .setCoefficients(m[3] - m[2]);
            planes[Bottom].setCoefficients(m[3] + m[1]);
            planes[Top]   .setCoefficients(m[3] - m[1]);
            planes[Left]  .setCoefficients(m[3] + m[0]);
            planes[Right] .setCoefficients(m[3] - m[0]);
        }


        [[nodiscard]] inline bool sphereIntersects(const Sphere &sphere) const
        {
            for (const auto &plane : planes)
            {
                if (plane.distance(sphere.center) < - sphere.radius)
                    return false;
            }
            return true;
        }

    };

}