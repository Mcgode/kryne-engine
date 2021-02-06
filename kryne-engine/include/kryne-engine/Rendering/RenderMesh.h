/**
 * @file
 * @author Max Godefroy
 * @date 31/01/2021.
 */

#pragma once


#include <kryne-engine/Core/Component.h>

class Entity;
class Camera;


#include <kryne-engine/Geometry/BufferGeometry.h>
#include <kryne-engine/Material/Material.h>


class RenderMesh: public Component {

public:

    RenderMesh(Entity *entity, shared_ptr<BufferGeometry> geometry, shared_ptr<Material> material);

    /**
     * Function called before the mesh is rendered
     * @param camera    The camera used for the rendering
     */
    virtual void onBeforeRender(const Camera *camera) {};

    /**
     * Returns the current geometry in the form of a shared pointer.
     */
    [[nodiscard]] const shared_ptr<BufferGeometry> &getGeometry() const {
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
    [[nodiscard]] const shared_ptr<Material> &getMaterial() const {
        return material;
    }

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

};

#include <kryne-engine/Camera/Camera.h>