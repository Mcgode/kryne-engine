/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#include "kryne-engine/Material/MeshStandardMaterial.h"

MeshStandardMaterial::MeshStandardMaterial(const StandardInitParameters &options):
    MeshMaterialCommon(make_unique<Shader>("Engine/Standard"))
{
    this->initializeCommon(options);
    this->setRoughness(options.roughness);
    this->setMetalness(options.metalness);
    this->setNormalMap(options.normalMap);
    this->setRoughnessMap(options.roughnessMap);
    this->setMetalnessMap(options.metalnessMap);
}
