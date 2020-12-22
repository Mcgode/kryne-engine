/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#ifndef KRYNE_ENGINE_MESHMATERIALCOMMON_H
#define KRYNE_ENGINE_MESHMATERIALCOMMON_H

#include <kryne-engine/Textures/Texture.h>

#include "Material.h"


using namespace std;


/**
 * A child class of Material containing the common values used by most materials.
 *
 * @note If deriving from this class, don't forget to call #initializeCommon() in the child class constructor
 */
class MeshMaterialCommon: public Material {

public:

    /**
     * A structure for holding the initial values of the material
     */
    struct InitParameters {

        /// The initial color of the material. Defaults to white: #ffffff
        vec3 color {1};

        /// The initial color/diffuse map of the material. None by default.
        shared_ptr<Texture> map = nullptr;

        /**
         * Base initializer
         */
        InitParameters() = default;

    };

public:

    /**
     * Change the value of the material diffuse color and updates the uniform value
     * @param newColor The new material color
     */
    void setColor(const vec3 &newColor);

    /**
     * Changes the diffuse map of the material, updating its uniform value accordingly, and enabling and disabling
     * shader map use (using define `USE_MAP`) accordingly.
     * @param newMap The new map value. Set to `nullptr` to disable
     */
    void setMap(const shared_ptr<Texture> &newMap);

protected:

    /*!@copydoc Material::Material()
     *
     * Calls Material::Material(unique_ptr<Shader>) directly. The call to #initializeCommon() must be done manually, to
     * allow for more initialization flexibility of children.
     */
    explicit MeshMaterialCommon(unique_ptr<Shader> shader);

    /**
     * Initializes the values of the common material values
     * @param options The initialization values.
     */
    void initializeCommon(const InitParameters &options);

protected:

    /// The diffuse color of the material
    vec3 color {};

    /// The diffuse map of the material
    shared_ptr<Texture> map {};

};


#endif //KRYNE_ENGINE_MESHMATERIALCOMMON_H
