/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#ifndef KRYNE_ENGINE_MESHMATERIALCOMMON_H
#define KRYNE_ENGINE_MESHMATERIALCOMMON_H

#include <optional>
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
    void setColor(const vec3 &newColor)
    {
        this->setMaterialBasicProperty(this->color, newColor, "color");
    }

    /**
     * Changes the diffuse map of the material, updating its uniform value accordingly, and enabling and disabling
     * shader map use (using define `USE_MAP`) accordingly.
     * @param newMap The new map value. Set to `nullptr` to disable
     */
    void setMap(const shared_ptr<Texture> &newMap)
    {
        this->setMaterialOptionalProperty(this->map, newMap, "map", "USE_MAP");
    }

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

    /**
     * Updates the value of a simple property and updates its uniform value if there was a change.
     * @tparam T            The property type. Must belong to UniformsHandler::UniformTypes.
     * @param property      he material instance property's reference.
     * @param value         The new value for the property.
     * @param uniformName   The uniform name corresponding to this property.
     */
    template<class T>
    inline void setMaterialBasicProperty(T &property, const T &value, const string &uniformName)
    {
        static_assert(!is_same<shared_ptr<Texture>, T>(), "Textures are not considered basic properties");

        if (property != value)
            this->setUniform(uniformName, value);

        property = value;
    }

    /**
     * Updates the value of a simple property and updates its uniform value if there was a change. Will also remove or
     * declare the attributed define depending on the new value of the property.
     * @param property      The material instance property's reference
     * @param newValue      The new value for the property
     * @param uniformName   The uniform name corresponding to this property
     * @param define
     */
    inline void setMaterialOptionalProperty(shared_ptr<Texture> &property,
                                            const shared_ptr<Texture> &newValue,
                                            const string &uniformName,
                                            const string &define);


    /**
     * @copydoc setMaterialOptionalProperty(shared_ptr<Texture> &, const shared_ptr<Texture> &, const string &, const string &)
     * @tparam T    The property type
     */
    template<class T>
    inline void setMaterialOptionalProperty(optional<T> &property,
                                            const optional<T> &newValue,
                                            const string &uniformName,
                                            const string &define)
    {
        static_assert(!is_same<shared_ptr<Texture>, T>(), "Textures are not allowed to be abstracted with optionals, pass the shared pointer directly");

        if (newValue != property)
        {
            if (!newValue.has_value())
                this->removeDefine(define);

            else if (!property.has_value())
                this->setDefine(define, "");

            if (newValue.has_value())
                this->setUniform(uniformName, newValue.value());
        }

        property = newValue;
    }

protected:

    /// The diffuse color of the material
    vec3 color {};

    /// The diffuse map of the material
    shared_ptr<Texture> map {};

};


#endif //KRYNE_ENGINE_MESHMATERIALCOMMON_H
