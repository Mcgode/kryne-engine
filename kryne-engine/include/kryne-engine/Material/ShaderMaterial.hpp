/**
 * @file
 * @author Max Godefroy
 * @date 06/04/2021.
 */

#pragma once


#include "Material.h"


/**
 * @brief A wrapper material for directly using shaders
 */
class ShaderMaterial : public Material {

public:

    /**
     * @brief Initializes the shader material.
     *
     * @param shader The shader for this material.
     */
    explicit ShaderMaterial(unique_ptr<Shader> shader) : Material(move(shader)) {}

    /// @brief Retrieves this material's name
    [[nodiscard]] const string &getName() const { return name; }

    /// @brief Changes this material's name
    void setName(const string &value) { ShaderMaterial::name = value; }

protected:

    // Override
    string materialName() override { return this->name; }

protected:

    /// The name of this material
    string name = "ShaderMaterial";

};


