/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#ifndef KRYNE_ENGINE_MESHLAMBERTMATERIAL_H
#define KRYNE_ENGINE_MESHLAMBERTMATERIAL_H

#include <optional>

#include "Material.h"

class MeshLambertMaterial : public Material {

public:

    struct InitParameters {

        vec3 color {1};

        optional<shared_ptr<Texture>> map;

        InitParameters() {}

    };

public:

    explicit MeshLambertMaterial(const InitParameters &options = InitParameters());

    void setColor(const vec3 &newColor);

    void setMap(const shared_ptr<Texture> &newMap);

protected:

    vec3 color {};

    shared_ptr<Texture> map {};

};


#endif //KRYNE_ENGINE_MESHLAMBERTMATERIAL_H
