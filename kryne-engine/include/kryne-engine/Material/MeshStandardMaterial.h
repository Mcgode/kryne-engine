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

        shared_ptr<Texture> normalMap;

        shared_ptr<Texture> roughnessMap;
        shared_ptr<Texture> metalnessMap;

        StandardInitParameters(): InitParameters() {}

    };

public:

    explicit MeshStandardMaterial(const StandardInitParameters &options = StandardInitParameters());

    void setRoughness(float newRoughness);

    void setMetalness(float newMetalness);

    void setNormalMap(const shared_ptr<Texture> &newMap);

    void setRoughnessMap(const shared_ptr<Texture> &newMap);

    void setMetalnessMap(const shared_ptr<Texture> &newMap);

protected:

    float roughness = 0;

    float metalness = 0;

    shared_ptr<Texture> normalMap;

    shared_ptr<Texture> roughnessMap;

    shared_ptr<Texture> metalnessMap;

};


#endif //KRYNE_ENGINE_MESHSTANDARDMATERIAL_H
