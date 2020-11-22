//
// Created by Max Godefroy on 21/11/2020.
//

#ifndef INC_3D_DEMOS_MESH_H
#define INC_3D_DEMOS_MESH_H

#include <kryne-engine/Geometry/BufferGeometry.h>
#include <kryne-engine/Material/Material.h>
#include <kryne-engine/Camera/Camera.h>

#include "Object3D.h"


using namespace std;

/**
 * A 3D object that can be rendered. <br>
 * Its geometry and material are saved as shared pointers because sharing the same geometry and/or material between
 * multiple meshes is an expected behaviour and an engine feature.
 */
class Mesh: public Object3D {

public:

    /**
     * Initializes a mesh.
     * @param geometry  The geometry for this mesh.
     * @param material  The material for this mesh.
     */
    Mesh(const shared_ptr<BufferGeometry> &geometry, const shared_ptr<Material> &material) :
        Object3D(),
        geometry(geometry),
        material(material)
        {};

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
        Mesh::geometry = newGeometry;
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
        Mesh::material = newMaterial;
    }

protected:

    /// The mesh geometry
    shared_ptr<BufferGeometry> geometry;

    /// The mesh material
    shared_ptr<Material> material;


public:

    void update(bool force) override
    {
        bool willUpdate = (this->matrixWorldNeedsUpdate || force) && this->visible;
        Object3D::update(force);
        if (willUpdate)
            this->material->getShader()->setMat4("matrixWorld", this->matrixWorld);
    }


};


#endif //INC_3D_DEMOS_MESH_H
