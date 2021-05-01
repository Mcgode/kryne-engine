/**
 * @file
 * @author Max Godefroy
 * @date 18/04/2021.
 */

#include "PerspectiveHelper.hpp"

PerspectiveHelper::PerspectiveHelper(Process *process, const weak_ptr<Entity> &camera) : Entity(process),
                                                                                         camera(camera)
{
    auto geometry = make_shared<BufferGeometry>();
    vector<vec3> rawPositions = {
            vec3(-1, -1, -1),
            vec3( 1, -1, -1),
            vec3(-1,  1, -1),
            vec3( 1,  1, -1),
            vec3(-1, -1,  1),
            vec3( 1, -1,  1),
            vec3(-1,  1,  1),
            vec3( 1,  1,  1),
    };
    vector<vec3> positions;

    auto matrix = ((Camera *)(camera.lock().get()))->getInverseProjectionMatrix();
    for (const auto &p : rawPositions)
    {
        vec4 v = matrix * vec4(p, 1);
        positions.push_back(vec3(v) / v.w);
    }

    geometry->setAttribute("position", make_unique<BufferAttribute>(Utils::flatMapFloatVectors(positions), 3));

    vector<uint32_t> indices = {
            0, 1,   1, 3,   0, 2,   2, 3,
            0, 4,   1, 5,   2, 6,   3, 7,
            4, 5,   5, 7,   4, 6,   6, 7,
    };
    geometry->setIndices(indices);

    auto material = make_shared<ShaderMaterial>(make_unique<Shader>("Engine/Basic"));
    material->setPrimitiveType(GL_LINES);
    material->setSide(DoubleSide);

    const auto renderMesh = this->addComponent<RenderMesh>(geometry, material);
    renderMesh->setFrustumCull(false);
}


