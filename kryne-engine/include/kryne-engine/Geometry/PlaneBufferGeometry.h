/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once


#include <kryne-engine/Utils/VectorMapping.hpp>

#include "BufferGeometry.h"


class PlaneBufferGeometry : public BufferGeometry {

public:

    PlaneBufferGeometry(float width = 1.f, float height = 1.f, uint32_t widthSegments = 1, uint32_t heightSegments = 1);

};


