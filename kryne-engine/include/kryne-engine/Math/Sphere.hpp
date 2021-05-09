/**
 * @file
 * @author Max Godefroy
 * @date 13/03/2021.
 */

#pragma once


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/component_wise.hpp>

#include "AxisAlignedBox.hpp"


namespace Math {

    /**
     * @brief A structure for handling a mathematical sphere object.
     */
    struct Sphere {

        /// The center of the sphere.
        vec3 center;

        /// The radius of the sphere.
        float radius;


        /**
         * @brief Initializes a new Sphere
         *
         * @param radius The initial radius of the sphere. Defaults to infinity.
         * @param center The initial sphere center. Defaults to (0, 0, 0).
         */
        explicit Sphere(float radius = std::numeric_limits<float>::infinity(), const vec3 &center = vec3()) : center(center), radius(radius) {}


        /**
         * @brief Expands a sphere to encompass the provided point.
         *
         * @param point The point to expand the sphere with.
         */
        inline void expandByPoint(vec3 point)
        {
            if (radius == std::numeric_limits<float>::infinity())
            {
                center = point;
                radius = 0.f;
                return;
            }

            float dist = distance(point, center);
            if (dist > radius)
            {
                radius = (radius + dist) / 2.f;
                float x = radius / dist;
                center = mix(point, center, x);
            }
        }


        /**
         * @brief Transforms the sphere by applying the provided matrix into a new Sphere.
         *
         * @param m The transformation matrix to apply to the new sphere.
         * @return A new Sphere structure instance, with its attributes updated.
         */
        [[nodiscard]] inline Sphere fromMatrix(const mat4 &m) const
        {
            vec3 squaredScale(length2(vec3(m[0])),
                              length2(vec3(m[1])),
                              length2(vec3(m[2])));
            float scale = sqrt(compMax(squaredScale));

            return Sphere(radius * scale,
                          vec3(m * vec4(center, 1.f)));

        }


        /**
         * @brief Updates the sphere parameters to bound the provided box.
         *
         * @param box An axis aligned box, which will be bounded by the sphere.
         */
        inline void fromBox(const AxisAlignedBox &box)
        {
            this->center = (box.min + box.max) * 0.5f;
            this->radius = 0.5f * distance(box.min, box.max);
        }

    };

}