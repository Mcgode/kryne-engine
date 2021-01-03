/**
 * @file
 * @author Max Godefroy
 * @date 21/11/2020
 */

#ifndef INC_KRYNE_ENGINE_MESH_H
#define INC_KRYNE_ENGINE_MESH_H

#include <kryne-engine/Material/Material.h>
#include <kryne-engine/Camera/Camera.h>


using namespace std;

/**
 * A 3D object that can be rendered. <br>
 * Its geometry and material are saved as shared pointers because sharing the same geometry and/or material between
 * multiple meshes is an expected behaviour and an Engine feature.
 */
class Mesh: public Transform {

public:

    /**
     * Initializes a mesh.
     * @param geometry  The geometry for this mesh.
     * @param material  The material for this mesh.
     */
    Mesh(const shared_ptr<BufferGeometry> &geometry, const shared_ptr<Material> &material, Entity *entity) :
            Transform(entity),
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

    //! @copydoc Object3D::updateTransform()
    void updateTransform(bool force) override;

    //! @copydoc Object3D::updateParents()
    const mat4 &updateParents(const Transform *caller) override;


};


#endif //INC_KRYNE_ENGINE_MESH_H
