/**
 * @file
 * @author Max Godefroy
 * @date 05/04/2021.
 */

#include "kryne-engine/Systems/LightingRegistrySystem.hpp"


LightingRegistrySystem::LightingRegistrySystem(Process *process) : System(process, PreRendering) {}


void LightingRegistrySystem::loopReset()
{
    this->ambientLights.clear();
    this->hemisphereLights.clear();
    this->directionalLights.clear();

    this->shouldRun = this->getAttachedProcess()->getGraphicContext()->getRenderer()->getRenderingMode() == ForwardRendering;
}


void LightingRegistrySystem::runSystem(Entity *entity)
{
    if (!shouldRun)
        return;

    for (const auto &renderMesh : entity->getComponents<RenderMesh>())
    {
        if (!renderMesh->isEnabled())
            continue;

        const auto material = renderMesh->getMaterial().get();
        this->updateAmbientLights(material);
        this->updateHemisphereLights(material);
        this->updateDirectionalLights(material);
    }
}


void LightingRegistrySystem::parseScene(Scene *scene, unordered_set<Entity *> &priorityEntities)
{
    for (Entity *entity : scene->getEntities())
    {
        if (!entity->isEnabled())
            continue;

        const auto light = dynamic_cast<Light *>(entity);
        if (light != nullptr)
        {
            priorityEntities.insert(light);

            switch (light->getType())
            {
                case Light::HemisphereLight:
                    this->hemisphereLights.push_back(dynamic_cast<HemisphereLight *>(light));
                    break;
                case Light::AmbientLight:
                    this->ambientLights.push_back(dynamic_cast<AmbientLight *>(light));
                    break;
                case Light::DirectionalLight:
                    this->directionalLights.push_back(dynamic_cast<DirectionalLight *>(light));
                    break;
                case Light::PointLight:
                    break;
            }
        }
    }
}


void LightingRegistrySystem::updateAmbientLights(Material *material)
{
    const auto size = this->ambientLights.size();
    if (size)
        material->setDefine("MAX_AMBIENT_LIGHTS", to_string(size));
    else
        material->removeDefine("MAX_AMBIENT_LIGHTS");

    for (size_t i = 0; i < this->ambientLights.size(); i++)
    {
        const auto light = this->ambientLights[i];
        float intensity = light->getIntensity();
        material->setUniform("ambientLights[" + to_string(i) + "].color", intensity * light->getColor());
    }
}


void LightingRegistrySystem::updateHemisphereLights(Material *material)
{
    const auto size = this->hemisphereLights.size();
    if (size)
        material->setDefine("MAX_HEMISPHERE_LIGHTS", to_string(size));
    else
        material->removeDefine("MAX_HEMISPHERE_LIGHTS");

    for (size_t i = 0; i < this->hemisphereLights.size(); i++)
    {
        const auto light = this->hemisphereLights[i];
        float intensity = light->getIntensity();
        material->setUniform("hemisphereLights[" + to_string(i) + "].skyColor", intensity * light->getSkyColor());
        material->setUniform("hemisphereLights[" + to_string(i) + "].groundColor", intensity * light->getGroundColor());
        material->setUniform("hemisphereLights[" + to_string(i) + "].direction", light->getWorldDirection());
    }
}


void LightingRegistrySystem::updateDirectionalLights(Material *material)
{
    const auto size = this->directionalLights.size();
    if (size)
        material->setDefine("MAX_DIRECTIONAL_LIGHTS", to_string(size));
    else
        material->removeDefine("MAX_DIRECTIONAL_LIGHTS");

    for (size_t i = 0; i < this->directionalLights.size(); i++)
    {
        const auto light = this->directionalLights[i];
        float intensity = light->getIntensity();
        material->setUniform("directionalLights[" + to_string(i) + "].color", intensity * light->getColor());
        material->setUniform("directionalLights[" + to_string(i) + "].direction", light->getWorldDirection());
    }
}
