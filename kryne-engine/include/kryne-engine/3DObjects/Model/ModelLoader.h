/**
 * @file
 * @author Max Godefroy
 * @date 26/07/2019
 */

#ifndef INC_KRYNE_ENGINE_MODEL_LOADER_H
#define INC_KRYNE_ENGINE_MODEL_LOADER_H

#include <vector>
#include <map>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "kryne-engine/3DObjects/BaseObject.h"
#include "ModelMesh.h"

class ModelLoader {

public:
    static vector<ModelMesh *> loadModel(const string &modelFilename);

private:
    static void loadNode(aiNode *node, const aiScene *scene, const string &directory, vector<ModelMesh *> *result,
                         map<string, Texture2D *> *loadedTextures);
};


#endif //INC_KRYNE_ENGINE_MODEL_LOADER_H
