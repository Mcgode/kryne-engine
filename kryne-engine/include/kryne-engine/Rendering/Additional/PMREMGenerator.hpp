/**
 * @file
 * @author Max Godefroy
 * @date 07/04/2021.
 */

#pragma once


#include <deque>
#include <algorithm>
#include <kryne-engine/Textures/EnvironmentMap.hpp>
#include <kryne-engine/Rendering/LoopRenderer.h>


/**
 * @brief A class that generates PMREMs
 */
class PMREMGenerator {

public:

    void runProcessing(LoopRenderer *renderer);

    /**
     * @brief Adds an environment map to the processing queue.
     *
     * @param map   The environment map to PMREM-ize.
     */
    void processMap(const shared_ptr<EnvironmentMap> &map);

    /**
     * @brief Check if the generator has maps left to process.
     *
     * @return true if #mapsToProcess is not empty.
     */
    [[nodiscard]] bool isProcessing() const { return !this->mapsToProcess.empty(); }

    /**
     * @brief Checks if the provided map is being processed.
     *
     * @param map   The environment map to check for.
     *
     * @return true if the map is in the process queue.
     */
    [[nodiscard]] bool isProcessing(const shared_ptr<EnvironmentMap> &map);

protected:

    /// The queue of maps to process.
    deque<shared_ptr<EnvironmentMap>> mapsToProcess {};

    /// The mutex for ensuring thread-safe queue operations
    mutex dequeMutex {};

};


