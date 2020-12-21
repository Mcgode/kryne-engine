/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#ifndef KRYNE_ENGINE_MESHSTANDARDMATERIAL_H
#define KRYNE_ENGINE_MESHSTANDARDMATERIAL_H

#include "MeshMaterialCommon.h"

class MeshStandardMaterial: public MeshMaterialCommon {

public:

    struct StandardInitParameters: public InitParameters {

        float roughness = 0.5;
        float metalness = 0.5;

        StandardInitParameters(): InitParameters() {}

    };

public:

    explicit MeshStandardMaterial(const StandardInitParameters &options = StandardInitParameters());

    void setRoughness(float newRoughness);

    void setMetalness(float newMetalness);

protected:

    float roughness = 0;
    float metalness = 0;

};


#endif //KRYNE_ENGINE_MESHSTANDARDMATERIAL_H
