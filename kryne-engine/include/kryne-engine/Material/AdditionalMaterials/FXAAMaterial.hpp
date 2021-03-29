/**
 * @file
 * @author Max Godefroy
 * @date 29/03/2021.
 */

#pragma once

#include "kryne-engine/Material/Material.h"


class FXAAMaterial : public Material {

public:

    FXAAMaterial() : Material(make_unique<Shader>("Engine/Additional/FXAAShader")) {}

};