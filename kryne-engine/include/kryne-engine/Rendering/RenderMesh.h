/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#pragma once


#include <kryne-engine/Core/Component.h>


#include "IRenderMesh.hpp"


class RenderMesh: public Component, virtual public IRenderMesh {

public:

    RenderMesh(Entity *entity, shared_ptr<BufferGeometry> geometry, shared_ptr<Material> material);

    /**
     * Function called before the mesh is rendered
     * @param camera    The camera used for the rendering
     */
    void onBeforeRender(const ICamera *camera) override {};

    /**
     * Returns the current geometry in the form of a shared pointer.
     */
    [[nodiscard]] const shared_ptr<BufferGeometry> &getGeometry() const override {
        return geometry;
    }

    /**
     * Overrides the current mesh geometry.
     * @param newGeometry   The new geometry for this mesh.
     */
    void setGeometry(const shared_ptr<BufferGeometry> &newGeometry) {
        this->geometry = newGeometry;
    }

    /**
     * Returns the current material in the form of a shared pointer.
     */
    [[nodiscard]] const shared_ptr<Material> &getMaterial() const override  { return material; }

    /**
     * Overrides the current mesh material.
     * @param newMaterial   The new material for this mesh.
     */
    void setMaterial(const shared_ptr<Material> &newMaterial) {
        this->material = newMaterial;
    }

protected:

    /// The mesh geometry
    shared_ptr<BufferGeometry> geometry;

    /// The mesh material
    shared_ptr<Material> material;


public:

    void transformDidUpdate() override;

    [[nodiscard]] bool isFrustumCull() const { return frustumCull; }

    void setFrustumCull(bool value) { RenderMesh::frustumCull = value; }

    [[nodiscard]] const Math::Sphere &getBoundingSphere() const { return boundingSphere; }

protected:

    bool frustumCull = true;

    Math::Sphere boundingSphere;


public:

    void renderComponentDetails() override;

};