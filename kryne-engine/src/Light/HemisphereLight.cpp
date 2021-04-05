/**
 * @file
 * @author Max Godefroy
 * @date 05/04/2021.
 */

#include "kryne-engine/Light/HemisphereLight.hpp"


HemisphereLight::HemisphereLight(Process *process,
                                 const vec3 &skyColor,
                                 const vec3 &groundColor,
                                 const vec3 &direction) :
    Light(process),
    skyColor(skyColor),
    groundColor(groundColor),
    direction(direction),
    worldDirection(direction) {}


void HemisphereLight::transformDidUpdate()
{
    Entity::transformDidUpdate();
    this->worldDirection = this->transform->getNormalMatrix() * this->direction;
}
