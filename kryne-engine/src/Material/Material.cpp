//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/Material/Material.h"

void Material::use()
{
    this->shader->use();
}


void Material::resetUse()
{
    this->shader->resetUse();
}
