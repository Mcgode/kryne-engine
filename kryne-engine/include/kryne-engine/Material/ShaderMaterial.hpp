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


// ====================
// Depth
// ====================

public:

    // Override
    Material *getDepthMaterial() override
    {
        return this->customDepthMat == nullptr ?
            &ShaderMaterial::defaultDepthMaterial() :
            this->customDepthMat.get();
    }

    /// @brief Retrieves the custom depth material of this material
    [[nodiscard]] Material *getCustomDepthMaterial() { return this->customDepthMat.get(); }

    /// @brief Updates the custom depth material of this material
    void setCustomDepthMaterial(unique_ptr<Material> material) { this->customDepthMat = move(material); }

protected:

    /// A custom depth material for this material.
    unique_ptr<Material> customDepthMat = nullptr;


// ====================
// Default materials
// ====================

public:

    /**
     * @brief A default depth material.
     */
    static ShaderMaterial &defaultDepthMaterial()
    {
        static ShaderMaterial mat(make_unique<Shader>("Engine/Depth"));
        return mat;
    }

};


