/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#ifndef KRYNE_ENGINE_MESHLAMBERTMATERIAL_H
#define KRYNE_ENGINE_MESHLAMBERTMATERIAL_H

#include <optional>

#include "MeshMaterialCommon.h"

class MeshLambertMaterial : public MeshMaterialCommon {

public:

    explicit MeshLambertMaterial(const InitParameters &options = InitParameters());

};


#endif //KRYNE_ENGINE_MESHLAMBERTMATERIAL_H
