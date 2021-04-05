/**
 * @file
 * @author Max Godefroy
 * @date 05/04/2021.
 */

#pragma once


#include <kryne-engine/Core/Entity.h>


/**
 * @brief An entity that will provide light to the scene.
 */
class Light: public Entity {

public:

    enum LightType {

        AmbientLight,

        HemisphereLight,

        DirectionalLight,

        PointLight,

    };

    [[nodiscard]] LightType getType() const { return this->type; }

protected:

    // Override
    explicit Light(Process *process, LightType type): Entity(process), type(type) {}

protected:

    LightType type;

};

