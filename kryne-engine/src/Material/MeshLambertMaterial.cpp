/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#include "kryne-engine/Material/MeshLambertMaterial.h"


MeshLambertMaterial::MeshLambertMaterial(const InitParameters &options):
    MeshMaterialCommon(make_unique<Shader>("Engine/Lambert"))
{
    this->initialize(options);
}