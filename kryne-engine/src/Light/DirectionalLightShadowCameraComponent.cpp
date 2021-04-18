/**
 * @file
 * @author Max Godefroy
 * @date 18/04/2021.
 */

#include "kryne-engine/Light/DirectionalLightShadowCameraComponent.hpp"

DirectionalLightShadowCameraComponent::DirectionalLightShadowCameraComponent(Entity *entity) : LogicComponent(entity)
{
    if (dynamic_cast<Camera *>(entity) == nullptr)
        throw runtime_error("The attached entity should be a Camera");
}

void DirectionalLightShadowCameraComponent::onUpdate()
{
    LogicComponent::onUpdate();

    if (this->mainCamera == nullptr)
        return;

    auto parent = dynamic_cast<DirectionalLight *>(this->getTransform()->getParent()->getEntity());
    if (parent == nullptr)
        return;

    auto projectionData = dynamic_cast<OrthographicProjectionData *>(this->camera()->getProjectionData());
    if (projectionData == nullptr)
        return;

    this->getTransform()->lookAt(parent->direction);
    auto rotationMatrix = glm::toMat4(this->getTransform()->getQuaternion());

    auto localMatrix = inverse(parent->getTransform()->getWorldMatrix() * rotationMatrix);
    auto transformMatrix = localMatrix *
                           this->mainCamera->getTransform()->getWorldMatrix() *
                           this->mainCamera->getInverseProjectionMatrix();

    vec3 frustumLocalPoints[] = {
        vec3(-1, -1, -1),
        vec3( 1, -1, -1),
        vec3(-1,  1, -1),
        vec3( 1,  1, -1),
        vec3(-1, -1,  1),
        vec3( 1,  1,  1),
        vec3(-1,  1,  1),
        vec3( 1,  1,  1),
    };

    vec4 p0_4 = transformMatrix * vec4(frustumLocalPoints[0], 1);
    vec3 p0 = vec3(p0_4) / p0_4.w;
    vec3 minV(p0), maxV(p0);

    for (const auto &point : frustumLocalPoints)
    {
        vec4 v = transformMatrix * vec4(point, 1);
        vec3 p = vec3(v) / v.w;
        maxV = glm::max(maxV, p);
        minV = glm::min(minV, p);
    }

    projectionData->setFrustum(minV, maxV);
}
