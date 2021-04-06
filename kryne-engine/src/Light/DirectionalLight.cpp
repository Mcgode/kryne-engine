/**
 * @file
 * @author Max Godefroy
 * @date 06/04/2021.
 */

#include "kryne-engine/Light/DirectionalLight.hpp"

DirectionalLight::DirectionalLight(Process *process,
                                   const vec3 &color,
                                   float intensity,
                                   const vec3 &direction)
        : Light(process, LightType::DirectionalLight),
          color(color),
          intensity(intensity),
          direction(normalize(direction)),
          worldDirection(normalize(direction))
{
    this->name = "DirectionalLight";
}


void DirectionalLight::transformDidUpdate()
{
    Entity::transformDidUpdate();
    worldDirection = this->getTransform()->getNormalMatrix() * this->direction;
}
