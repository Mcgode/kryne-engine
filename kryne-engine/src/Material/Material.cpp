//
// Created by Max Godefroy on 21/11/2020.
//

#include "kryne-engine/Material/Material.h"


void Material::prepareShader(const BufferGeometry *geometry)
{
    if (this->shader->getNeedsUpdate())
        this->shader->notifyUniformLocationsNeedUpdate();
    this->compiler->checkShaderCompile(this->shader.get(), geometry);
    this->shader->use();
}
