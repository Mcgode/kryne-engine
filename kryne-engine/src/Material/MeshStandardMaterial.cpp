/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#include "kryne-engine/Material/MeshStandardMaterial.h"

MeshStandardMaterial::MeshStandardMaterial(const StandardInitParameters &options):
    MeshMaterialCommon(make_unique<Shader>("Engine/Standard"))
{
    this->initialize(options);
    this->setRoughness(options.roughness);
    this->setMetalness(options.metalness);
}


void MeshStandardMaterial::setRoughness(float newRoughness)
{
    if (this->roughness != newRoughness)
        this->setUniform("roughness", newRoughness);

    this->roughness = newRoughness;
}


void MeshStandardMaterial::setMetalness(float newMetalness)
{
    if (this->metalness != newMetalness)
        this->setUniform("metalness", newMetalness);

    this->metalness = newMetalness;
}

void MeshStandardMaterial::setRoughnessMap(const shared_ptr<Texture> &newMap)
{
    if (newMap != this->roughnessMap)
    {
        if (newMap == nullptr)
            this->removeDefine("USE_ROUGHNESS_MAP");

        else if (this->roughnessMap == nullptr)
            this->setDefine("USE_ROUGHNESS_MAP", "");

        this->setUniform("roughnessMap", newMap);
    }

    this->roughnessMap = newMap;
}

void MeshStandardMaterial::setMetalnessMap(const shared_ptr<Texture> &newMap)
{
    if (newMap != this->metalnessMap)
    {
        if (newMap == nullptr)
            this->removeDefine("USE_METALNESS_MAP");

        else if (this->metalnessMap == nullptr)
            this->setDefine("USE_METALNESS_MAP", "");

        this->setUniform("metalnessMap", newMap);
    }

    this->metalnessMap = newMap;
}
