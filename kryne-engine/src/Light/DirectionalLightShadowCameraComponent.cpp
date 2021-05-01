/**
 * @file
 * @author Max Godefroy
 * @date 18/04/2021.
 */

#include <kryne-engine/Math/Sphere.hpp>

#include "kryne-engine/Light/DirectionalLightShadowCameraComponent.hpp"


DirectionalLightShadowCameraComponent::DirectionalLightShadowCameraComponent(Entity *entity, uint8 index)
        : LogicComponent(entity), index(index)
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

    const auto &data = parent->shadowMapData[this->index];
    if (data == nullptr)
        return;

    this->getTransform()->lookAt(parent->direction, vec3(0, 1, 0), true);
    auto rotationMatrix = glm::toMat4(this->getTransform()->getQuaternion());

    auto localMatrix = inverse(parent->getTransform()->getWorldMatrix() * rotationMatrix);
    auto worldMatrix = inverse(this->mainCamera->getProjectionMatrix() * this->mainCamera->getViewMatrix());

    const auto m = mainCamera->getInverseProjectionMatrix();
    auto v1 = m * vec4(0, 0, -1, 1),
         v2 = m * vec4(0, 0,  1, 1);
    auto p1 = vec3(v1) / v1.w,
         p2 = vec3(v2) / v2.w;
    float baseDepth = glm::distance(p1, p2);
    float depth = baseDepth;
    if (parent->maxShadowDistance > 0.f)
        depth = glm::min(depth, parent->maxShadowDistance);

    const float total = (float) (1 << parent->cascadedShadowMaps) - 1;
    const float a = (float) (1 << this->index) - 1,
                b = (float) (1 << (this->index + 1)) - 1;
    const auto da = glm::mix(p1, p2, depth / baseDepth * a / total),
               db = glm::mix(p1, p2, depth / baseDepth * b / total);
    auto va = mainCamera->getProjectionMatrix() * vec4(da, 1),
         vb = mainCamera->getProjectionMatrix() * vec4(db, 1);
    const float za = va.z / va.w,
                zb = vb.z / vb.w;

    vec3 frustumLocalPoints[] = {
        vec3(-1, -1, za),
        vec3( 1, -1, za),
        vec3(-1,  1, za),
        vec3( 1,  1, za),
        vec3(-1, -1, zb),
        vec3( 1, -1, zb),
        vec3(-1,  1, zb),
        vec3( 1,  1, zb),
    };

    vec3 frustumCenter(0);
    for (auto &point : frustumLocalPoints)
    {
        vec4 v = worldMatrix * vec4( point, 1.f );
        point = vec3(v) / v.w;
        frustumCenter += point * 0.125f;
    }

    float radius = std::numeric_limits<float>::min();
    for (const auto &point : frustumLocalPoints)
    {
        float distance = glm::length(point - frustumCenter);
        radius = glm::max(radius, distance);
    }

    Math::Sphere sphere(radius, frustumCenter);
    const auto orthoSphere = sphere.fromMatrix(localMatrix);

    vec3 minOrtho = orthoSphere.center - orthoSphere.radius;
    vec3 maxOrtho = orthoSphere.center + orthoSphere.radius;

    projectionData->setFrustum(minOrtho, maxOrtho);

    const auto texelSize = projectionData->getTexelSize(vec2(data->shadowFramebuffer->getSize()));
    const auto offset = projectionData->getOffset();
    auto roundedOffset = glm::round(vec2(offset) / texelSize) * texelSize;
    projectionData->setOffset(vec3(roundedOffset, offset.z));
}
