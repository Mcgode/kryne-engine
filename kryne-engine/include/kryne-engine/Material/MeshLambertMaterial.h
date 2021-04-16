/**
 * @file
 * @author Max Godefroy
 * @date 21/12/2020.
 */

#ifndef KRYNE_ENGINE_MESHLAMBERTMATERIAL_H
#define KRYNE_ENGINE_MESHLAMBERTMATERIAL_H


#include "MeshMaterialCommon.h"

/**
 * A material class with Lambertian properties
 */
class MeshLambertMaterial : public MeshMaterialCommon {

public:

    /**
     * Initializes the lambertian material
     * @param options The material initial properties
     */
    explicit MeshLambertMaterial(const InitParameters &options = InitParameters());

protected:

    string materialName() override { return "Lambert"; }

    void dearImGuiData(Process *process) override;

};


#endif //KRYNE_ENGINE_MESHLAMBERTMATERIAL_H
