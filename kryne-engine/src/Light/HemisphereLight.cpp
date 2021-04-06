/**
 * @file
 * @author Max Godefroy
 * @date 05/04/2021.
 */

#include "kryne-engine/Light/HemisphereLight.hpp"


HemisphereLight::HemisphereLight(Process *process,
                                 const vec3 &skyColor,
                                 const vec3 &groundColor,
                                 float intensity,
                                 const vec3 &direction) :
    Light(process, LightType::HemisphereLight),
    skyColor(skyColor),
    groundColor(groundColor),
    intensity(intensity),
    direction(normalize(direction)),
    worldDirection(normalize(direction)) {}


void HemisphereLight::transformDidUpdate()
{
    Entity::transformDidUpdate();
    this->worldDirection = this->transform->getNormalMatrix() * this->direction;
}
