//
// Created by max on 26/07/2019.
//

#include "kryne-engine/3DObjects/Model/ModelLoader.h"

vector<ModelMesh *> ModelLoader::loadModel(const string &modelFilename)
{
    Assimp::Importer importer;

    const auto mScene = importer.ReadFile("Model/" + modelFilename,
                                         aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!mScene || (mScene->mFlags && AI_SCENE_FLAGS_INCOMPLETE) || !mScene->mRootNode) {
        cerr << "Assimp error: " << importer.GetErrorString() << endl;
        exit(EXIT_FAILURE);
    }

    string directory = ("Model/" + modelFilename).substr(0, ("Model/" + modelFilename).find_last_of('/'));
    vector<ModelMesh *> result;
    map<string, Texture2D *> loadedTextures;

    loadNode(mScene->mRootNode, mScene, directory, &result, &loadedTextures);

    return result;
}


void ModelLoader::loadNode(aiNode *node, const aiScene *scene, const string &directory, vector<ModelMesh *> *result,
                           map<string, Texture2D *> *loadedTextures)
{

    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        result->push_back(new ModelMesh(mesh, scene, directory, loadedTextures));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        loadNode(node->mChildren[i], scene, directory, result, nullptr);
    }
}
