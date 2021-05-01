/**
 * @file
 * @author Max Godefroy
 * @date 18/04/2021.
 */

#pragma once


#include <kryne-engine/Core/LogicComponent.h>
#include <kryne-engine/Light/DirectionalLight.hpp>
#include <kryne-engine/Camera/OrthographicProjectionData.hpp>


class DirectionalLightShadowCameraComponent : public LogicComponent {

public:

    explicit DirectionalLightShadowCameraComponent(Entity *entity, uint8 index);

    void onUpdate() override;

    void setMainCamera(Camera *main) { this->mainCamera = main; }

protected:

    [[nodiscard]] inline Camera *camera() const { return (Camera *)this->associatedEntity; }

protected:

    uint8 index;

    Camera *mainCamera = nullptr;

};


