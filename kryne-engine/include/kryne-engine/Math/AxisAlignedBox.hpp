/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#pragma once

#include <glm/vec3.hpp>
#include <glm/common.hpp>


using namespace glm;


namespace Math {

    /**
     * @brief A structure representing an axis-aligned 3D box
     */
    struct AxisAlignedBox {

        /// The minimum coordinates vector.
        vec3 min;

        /// The maximum coordinates vector.
        vec3 max;


        /**
         * @brief Initializes a box.
         *
         * @param min The initial minimum coordinates. Defaults to max float value.
         * @param max The initial minimum coordinates. Defaults to min float value.
         */
        explicit AxisAlignedBox(const vec3 &min = vec3(std::numeric_limits<float>::max()),
                                const vec3 &max = vec3(std::numeric_limits<float>::lowest())) :
            min(min), max(max) {}


        /**
         * @brief Expands the box to encompass the provided point.
         *
         * @param point The point to expand this box with.
         */
        inline void expandByPoint(vec3 point)
        {
            this->min = glm::min(this->min, point);
            this->max = glm::max(this->max, point);
        }

    };

}
