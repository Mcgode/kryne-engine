/**
 * @file
 * @author Max Godefroy
 * @date 07/04/2021.
 */

#include "kryne-engine/Textures/EnvironmentMap.hpp"

void EnvironmentMap::setIBL(const shared_ptr<CubeTexture> &newEnvMap)
{
    this->iblReady = true;
    this->environmentMap = newEnvMap;
}
