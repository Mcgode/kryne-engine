/**
 * @file
 * @author Max Godefroy
 * @date 08/04/2021.
 */

#pragma once


#include "BufferGeometry.h"


class TorusKnotBufferGeometry : public BufferGeometry {

public:

    explicit TorusKnotBufferGeometry(float radius = 1, float tube = 0.4,
                                     uint32_t tubularSegments = 64, uint32_t radialSegments = 8,
                                     uint32_t p = 2, uint32_t q = 3);

protected:

    static vec3 computePositionOnCurve(float u, uint32_t p, uint32_t q, float radius);

};


