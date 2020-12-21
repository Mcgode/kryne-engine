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
