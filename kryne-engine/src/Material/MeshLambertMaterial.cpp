/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#include "kryne-engine/Material/MeshLambertMaterial.h"


MeshLambertMaterial::MeshLambertMaterial(const InitParameters &options):
    Material(make_unique<Shader>("Engine/Lambert"))
{
    this->setMap(options.map.value_or(nullptr));
    this->setColor(options.color);
}


void MeshLambertMaterial::setColor(const vec3 &newColor)
{
    if (newColor != this->color)
        this->setUniform("color", this->color);
    this->color = newColor;
}


void MeshLambertMaterial::setMap(const shared_ptr<Texture> &newMap)
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
