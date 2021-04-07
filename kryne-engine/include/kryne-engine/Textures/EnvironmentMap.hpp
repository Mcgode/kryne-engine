/**
 * @file
 * @author Max Godefroy
 * @date 07/04/2021.
 */

#pragma once


#include "CubeTexture.h"


/**
 * @brief A wrapper for cube textures
 */
class EnvironmentMap {

public:

    /**
     * @brief Initializes the environment map.
     *
     * @param environmentMap    The cube texture for the environment map
     */
    explicit EnvironmentMap(const shared_ptr<CubeTexture> &environmentMap) : environmentMap(environmentMap) {}

    /**
     * @brief Initializes an environment map based on the provided files.
     *
     * @param filenames     The list of filenames to load.
     */
    static shared_ptr<EnvironmentMap> loadFiles(const vector<string>& filenames)
    {
        return make_shared<EnvironmentMap>(CubeTexture::loadFiles(filenames));
    }

    /**
     * @brief De-referencing operation.
     *
     * @return A reference of the underlying CubeTexture environment map.
     */
    shared_ptr<CubeTexture> &operator* () { return this->environmentMap; }

    /**
     * @brief Pointer forwarding operation.
     *
     * @return A to the underlying CubeTexture environment map.
     */
    CubeTexture *operator->() { return this->environmentMap.get(); }

    [[nodiscard]] bool isIblReady() const { return iblReady; }

    void setIBL(const shared_ptr<CubeTexture> &newEnvMap);

protected:

    /// The underlying cube texture
    shared_ptr<CubeTexture> environmentMap;

    /// If true, the environment map was prefiltered.
    bool iblReady = false;

};


