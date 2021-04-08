/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#ifndef KRYNE_ENGINE_MESHSTANDARDMATERIAL_H
#define KRYNE_ENGINE_MESHSTANDARDMATERIAL_H


#include <kryne-engine/Textures/EnvironmentMap.hpp>
#include "MeshMaterialCommon.h"


/**
 * A material class with default PBR properties.
 */
class MeshStandardMaterial: public MeshMaterialCommon {

public:

    /**
     * @copydoc MeshMaterialCommon::InitParameters
     *
     * Extended to fit in the standard material properties.
     */
    struct StandardInitParameters: public InitParameters {

        /// The initial roughness of the material. Defaults to 1.
        float roughness = 1;

        /// The initial metalness of the material. Defaults to 0.
        float metalness = 0;

        /// The initial normal map of the material. None by default.
        shared_ptr<Texture> normalMap;

        /// The initial roughness map of the material. None by default.
        shared_ptr<Texture> roughnessMap;

        /// The initial metalness map of the material. None by default.
        shared_ptr<Texture> metalnessMap;

        /**
         * Base initializer
         */
        StandardInitParameters(): InitParameters() {}

    };

public:

    /**
     * Initializes the standard material with base values
     * @param options The material initial properties
     */
    explicit MeshStandardMaterial(const StandardInitParameters &options = StandardInitParameters());

    /**
     * Change the value of the material roughness and updates the uniform value
     * @param newRoughness  The new material roughness
     */
    void setRoughness(float newRoughness)
    {
        this->setMaterialBasicProperty(this->roughness, newRoughness, "roughness");
    }

    /**
     * Change the value of the material metalness and updates the uniform value
     * @param newMetalness  The new material metalness
     */
    void setMetalness(float newMetalness)
    {
        this->setMaterialBasicProperty(this->metalness, newMetalness, "metalness");
    }

    /**
     * Changes the normal map of the material, updating its uniform value accordingly, and enabling and disabling
     * shader map use (using define `USE_NORMAL_MAP`) accordingly.
     * @param newMap    The new map value. Set to `nullptr` to disable
     */
    void setNormalMap(const shared_ptr<Texture> &newMap)
    {
        this->setMaterialOptionalProperty(this->normalMap, newMap, "normalMap", "USE_NORMAL_MAP");
    }

    /**
     * Changes the roughness map of the material, updating its uniform value accordingly, and enabling and disabling
     * shader map use (using define `USE_ROUGHNESS_MAP`) accordingly.
     * @param newMap    The new map value. Set to `nullptr` to disable
     */
    void setRoughnessMap(const shared_ptr<Texture> &newMap)
    {
        this->setMaterialOptionalProperty(this->roughnessMap, newMap, "roughnessMap", "USE_ROUGHNESS_MAP");
    }

    /**
     * Changes the metalness map of the material, updating its uniform value accordingly, and enabling and disabling
     * shader map use (using define `USE_METALNESS_MAP`) accordingly.
     * @param newMap    The new map value. Set to `nullptr` to disable
     */
    void setMetalnessMap(const shared_ptr<Texture> &newMap)
    {
        this->setMaterialOptionalProperty(this->metalnessMap, newMap, "metalnessMap", "USE_METALNESS_MAP");
    }

    /// @brief Retrieves whether the envmap should be the same as the scene skybox.
    [[nodiscard]] bool isUseSkyboxEnvmap() const { return useSkyboxEnvmap; }

    /// @brief Changes whether the envmap should be the same as the scene skybox.
    void setUseSkyboxEnvmap(bool value) { MeshStandardMaterial::useSkyboxEnvmap = value; }

    /// @brief Retrieves the current environment map
    [[nodiscard]] const shared_ptr<EnvironmentMap> &getEnvMap() const { return envMap; }

    /// @brief Changes the current environment map
    void setEnvMap(const shared_ptr<EnvironmentMap> &map)
    {
        this->envMap = map;
        this->setMaterialOptionalProperty(this->iblEnvMap, map != nullptr ? map->getIblEnvMap() : nullptr,
                                          "environmentMap", "USE_ENVMAP");
    }

    /// @brief Changes the current environment map intensity.
    void setEnvMapIntensity(float intensity)
    {
        this->setMaterialBasicProperty(this->envMapIntensity, intensity, "envMapIntensity");
    }

protected:

    // Override
    void beforeUpload(const BufferGeometry *geometry) override;

    // Override
    string materialName() override { return "Standard"; }

    // Override
    void dearImGuiData() override;

protected:

    /// The material roughness.
    float roughness = 0;

    /// The material metalness.
    float metalness = 0;

    /// The material normal map.
    shared_ptr<Texture> normalMap;

    /// The material roughness map.
    shared_ptr<Texture> roughnessMap;

    /// The material metalness map.
    shared_ptr<Texture> metalnessMap;

    /// Set to true to use the scene skybox as envmap.
    bool useSkyboxEnvmap = true;

    /// The IBL cubemap
    shared_ptr<Texture> iblEnvMap;

    /// The environment map for this material
    shared_ptr<EnvironmentMap> envMap;

    /// The intensity of the environment map.
    float envMapIntensity;

};


#endif //KRYNE_ENGINE_MESHSTANDARDMATERIAL_H
