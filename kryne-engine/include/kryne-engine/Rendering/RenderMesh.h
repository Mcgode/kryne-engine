/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#pragma once


#include <kryne-engine/Core/Component.h>
#include <kryne-engine/Geometry/BufferGeometry.h>
#include <kryne-engine/Material/Material.h>
#include <kryne-engine/Camera/Camera.h>


class RenderMesh: public Component {

public:

    RenderMesh(Entity *entity, shared_ptr<BufferGeometry> geometry, shared_ptr<Material> material);

    /**
     * @brief Function called before the mesh is rendered
     *
     * @param camera    The camera used for the rendering
     */
    virtual void onBeforeRender(const Camera *camera) {}

    /**
     * Returns the current geometry in the form of a shared pointer.
     */
    [[nodiscard]] const shared_ptr<BufferGeometry> &getGeometry() const { return geometry; }

    /**
     * Overrides the current mesh geometry.
     * @param newGeometry   The new geometry for this mesh.
     */
    void setGeometry(const shared_ptr<BufferGeometry> &newGeometry) { this->geometry = newGeometry; }

    /**
     * Returns the current material in the form of a shared pointer.
     */
    [[nodiscard]] const shared_ptr<Material> &getMaterial() const  { return material; }

    /**
     * Overrides the current mesh material.
     * @param newMaterial   The new material for this mesh.
     */
    void setMaterial(const shared_ptr<Material> &newMaterial) { this->material = newMaterial; }

protected:

    /// The mesh geometry
    shared_ptr<BufferGeometry> geometry;

    /// The mesh material
    shared_ptr<Material> material;


public:

    // Override
    void transformDidUpdate() override;

    /**
     * @brief Retrieves whether this mesh should be frustum culled or not
     */
    [[nodiscard]] bool isFrustumCull() const { return frustumCull; }

    /**
     * @brief Enables or disables frustum culling.
     */
    void setFrustumCull(bool value) { RenderMesh::frustumCull = value; }

    /**
     * @brief Retrieves the current bounding sphere.
     */
    [[nodiscard]] const Math::Sphere &getBoundingSphere() const { return boundingSphere; }

protected:

    /// Can this render mesh be frustum culled
    bool frustumCull = true;

    /// The bounding sphere of this mesh.
    Math::Sphere boundingSphere;


public:

    // Override
    void renderComponentDetails() override;

};