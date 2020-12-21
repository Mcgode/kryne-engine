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


class MeshMaterialCommon: public Material {

public:

    struct InitParameters {

        vec3 color {1};

        optional<shared_ptr<Texture>> map;

        InitParameters() = default;

    };

public:

    void setColor(const vec3 &newColor);

    void setMap(const shared_ptr<Texture> &newMap);

protected:

    explicit MeshMaterialCommon(unique_ptr<Shader> shader);

    void initialize(const InitParameters &options);

protected:

    vec3 color {};

    shared_ptr<Texture> map {};

};


#endif //KRYNE_ENGINE_MESHMATERIALCOMMON_H
