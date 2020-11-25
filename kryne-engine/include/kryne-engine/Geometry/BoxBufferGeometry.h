/**
 * @file
 * @author Max Godefroy
 * @date 22/11/2020
 */

#ifndef INC_3D_DEMOS_BOXBUFFERGEOMETRY_H
#define INC_3D_DEMOS_BOXBUFFERGEOMETRY_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <kryne-engine/Constants/CubeVertexData.h>
#include <kryne-engine/Utils/VectorFlattening.h>
#include "BufferGeometry.h"


/**
 * A buffer geometry preloaded with geometry data for a box object.
 */
class BoxBufferGeometry : public BufferGeometry {

public:

    /**
     * Initializes a box geometry
     * @param xSize     The total x size of the box
     * @param ySize     The total y size of the box
     * @param zSize     The total z size of the box
     */
    explicit BoxBufferGeometry(float xSize = 1.0f, float ySize = 1.0f, float zSize = 1.0f);

};


#endif //INC_3D_DEMOS_BOXBUFFERGEOMETRY_H
