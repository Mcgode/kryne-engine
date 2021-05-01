/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#include "kryne-engine/Material/MeshMaterialCommon.h"

void MeshMaterialCommon::initializeCommon(const InitParameters &options)
{
    // If color is (0, 0, 0), the uniform won't be created, but it's fine, since OpenGL will give it the default value
    // of (0, 0, 0) anyway.
    this->setColor(options.color);
    this->setMap(options.map);
}


MeshMaterialCommon::MeshMaterialCommon(unique_ptr<Shader> shader) : Material(move(shader)) {}


void MeshMaterialCommon::setMaterialOptionalProperty(shared_ptr<Texture> &property,
                                                     const shared_ptr<Texture> &newValue,
                                                     const string &uniformName,
                                                     const string &define)
{
    if (newValue != property)
    {
        if (newValue == nullptr)
            this->removeDefine(define);

        else if (property == nullptr)
            this->setDefine(define, "");

        this->setUniform(uniformName, newValue);
    }

    property = newValue;
}
