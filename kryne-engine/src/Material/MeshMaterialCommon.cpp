/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#include "kryne-engine/Material/MeshMaterialCommon.h"

void MeshMaterialCommon::initialize(const InitParameters &options)
{
    this->setColor(options.color);
    this->setMap(options.map.value_or(nullptr));
}


void MeshMaterialCommon::setColor(const vec3 &newColor)
{
    if (this->color != newColor)
        this->setUniform("color", newColor);

    this->color = newColor;
}

void MeshMaterialCommon::setMap(const shared_ptr<Texture> &newMap)
{
    if (newMap != this->map)
    {
        if (newMap == nullptr)
            this->removeDefine("USE_MAP");

        else if (this->map == nullptr)
            this->setDefine("USE_MAP", "");

        this->setUniform("map", newMap);
    }

    this->map = newMap;
}

MeshMaterialCommon::MeshMaterialCommon(unique_ptr<Shader> shader) : Material(move(shader)) {}
