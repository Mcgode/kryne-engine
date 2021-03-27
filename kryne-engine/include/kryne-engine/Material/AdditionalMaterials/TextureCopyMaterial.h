/**
 * @file
 * @author Max Godefroy
 * @date 15/03/2021.
 */

#pragma once


#include "kryne-engine/Material/Material.h"


class TextureCopyMaterial : public Material {

public:

    TextureCopyMaterial() : Material(make_unique<Shader>("Engine/Additional/CopyShader")) {}

protected:

    shared_ptr<Texture> textureToCopy;

};


