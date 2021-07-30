/**
 * @file
 * @author max
 * @date 7/30/21.
 */

#pragma once


#include <glm/vec2.hpp>


namespace KryneEngine
{
    class Window
    {
    public:
        virtual glm::ivec2 GetSize() = 0;
    };
}