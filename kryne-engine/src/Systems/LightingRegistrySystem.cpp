/**
 * @file
 * @author Max Godefroy
 * @date 05/04/2021.
 */

#include "kryne-engine/Systems/LightingRegistrySystem.hpp"


LightingRegistrySystem::LightingRegistrySystem(Process *process) : System(process, PreRendering) {}


void LightingRegistrySystem::loopReset()
{
    this->hemisphereLights.clear();

    this->shouldRun = this->getAttachedProcess()->getGraphicContext()->getRenderer()->getRenderingMode() == ForwardRendering;
}


void LightingRegistrySystem::runSystem(Entity *entity)
{
    if (!shouldRun)
        return;

    for (const auto &renderMesh : entity->getComponents<RenderMesh>())
    {
        const auto material = renderMesh->getMaterial().get();
        this->updateHemisphereLights(material);
    }
}


void LightingRegistrySystem::parseScene(Scene *scene, unordered_set<Entity *> &priorityEntities)
{
    for (Entity *entity : scene->getEntities())
    {
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
                    break;
                case Light::DirectionalLight:
                    break;
                case Light::PointLight:
                    break;
            }
        }
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
