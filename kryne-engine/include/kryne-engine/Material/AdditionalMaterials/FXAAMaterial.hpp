/**
 * @file
 * @author Max Godefroy
 * @date 29/03/2021.
 */

#pragma once

#include "kryne-engine/Material/Material.h"


/**
 * @brief A FXAA shader material, for use in post process.
 */
class FXAAMaterial : public Material {

public:

    /**
     * @brief Initializes a FXAA shader material.
     *
     * @details
     * Default values are:
     *  - #subpixelQuality = 0.75
     *  - #edgeThreshold = 0.125
     *  - #edgeThresholdMin = 0.0322
     */
    FXAAMaterial() : Material(make_unique<Shader>("Engine/Additional/FXAAShader"))
    {
        setSubpixelQuality(0.75);
        setEdgeThresholdMin(0.0322);
        setEdgeThreshold(0.125);
    }

    /// @brief Retrieves #subpixelQuality
    float getSubpixelQuality() const { return subpixelQuality; }

    /**
     * @brief Sets the FXAA subpixel quality value.
     *
     * @details
     * Choose the amount of sub-pixel aliasing removal.
     * This can effect sharpness.
     *      1.00 - upper limit (softer)
     *      0.75 - default amount of filtering
     *      0.50 - lower limit (sharper, less sub-pixel aliasing removal)
     *      0.25 - almost off
     *      0.00 - completely off
     */
    void setSubpixelQuality(float value)
    {
        Material::setDefine("SUBPIXEL_QUALITY", to_string(value));
        FXAAMaterial::subpixelQuality = value;
    }

    /// @brief Retrieves #edgeThresholdMin
    float getEdgeThresholdMin() const { return edgeThresholdMin; }

    /**
     * @brief Sets the FXAA edge threshold min value
     *
     * @details
     * Trims the algorithm from processing darks.
     *      0.0833 - upper limit (default, the start of visible unfiltered edges)
     *      0.0625 - high quality (faster)
     *      0.0312 - visible limit (slower)
     */
    void setEdgeThresholdMin(float value)
    {
        Material::setDefine("EDGE_THRESHOLD_MIN", to_string(value));
        FXAAMaterial::edgeThresholdMin = value;
    }

    /// @brief Retrieves #edgeThreshold
    float getEdgeThreshold() const { return edgeThreshold; }

    /**
     * @brief Sets the FXAA edge threshold value
     *
     * @details
     * The minimum amount of local contrast required to apply algorithm.
     *      0.333 - too little (faster)
     *      0.250 - low quality
     *      0.166 - default
     *      0.125 - high quality
     *      0.063 - overkill (slower)
     */
    void setEdgeThreshold(float value)
    {
        Material::setDefine("EDGE_THRESHOLD_MAX", to_string(value));
        FXAAMaterial::edgeThreshold = value;
    }

    // Override
    Material *getDepthMaterial() override { return nullptr; }

protected:

    string materialName() override { return "FXAA"; }

protected:

    /// FXAA subpixel quality
    float subpixelQuality;

    /// FXAA edge threshold min value
    float edgeThresholdMin;

    /// FXAA edge threshold scale value
    float edgeThreshold;

};