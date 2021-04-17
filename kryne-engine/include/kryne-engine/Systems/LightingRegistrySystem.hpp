/**
 * @file
 * @author Max Godefroy
 * @date 05/04/2021.
 */

#pragma once


#include <kryne-engine/Core/System.h>

#include <kryne-engine/Light/AmbientLight.hpp>
#include <kryne-engine/Light/HemisphereLight.hpp>
#include <kryne-engine/Light/DirectionalLight.hpp>

#include <kryne-engine/Rendering/Processes/ShadowMappingProcess.hpp>


/**
 * @brief A system charged with registering all the lights of the scene, and applying them in forward rendering.
 */
class LightingRegistrySystem: public System {

public:

    /**
     * @brief Initializes the lighting registry
     */
    explicit LightingRegistrySystem(Process *process);

    void runSystem(Entity *entity) override;

    void loopReset() override;

    void parseScene(Scene *scene, unordered_set<Entity *> &priorityEntities) override;

    virtual ~LightingRegistrySystem();

protected:

    void updateAmbientLights(Material *material);

    void updateHemisphereLights(Material *material);

    void updateDirectionalLights(Material *material);

protected:

    bool shouldRun = false;

    vector<AmbientLight *> ambientLights;

    vector<HemisphereLight *> hemisphereLights;

    vector<DirectionalLight *> directionalLights;

    ShadowMappingProcess *shadowProcess;


friend ShadowMappingProcess;

};


