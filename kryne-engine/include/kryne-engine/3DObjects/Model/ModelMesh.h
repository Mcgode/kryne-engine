/**
 * @file
 * @author Max Godefroy
 * @date 29/07/2019
 */

#ifndef INC_KRYNE_ENGINE_MODELMESH_H
#define INC_KRYNE_ENGINE_MODELMESH_H

#include <vector>
#include <map>
#include <string>

#include <assimp/scene.h>
#include <glm/glm.hpp>

#include <kryne-engine/Core/VertexArray.h>
#include <kryne-engine/Textures/Texture2D.h>


using namespace std;

class ModelMesh {

public:
    explicit ModelMesh(aiMesh *mesh, const aiScene *scene, const string& directory, map<string, Texture2D *> *loadedTextures);

    VertexArray *getVertexArray() const;

    Texture2D *getDiffuseMap() const;

    Texture2D *getSpecularMap() const;

    Texture2D *getNormalMap() const;

    Texture2D *getHeightMap() const;

private:
    void retrieveGeometry(aiMesh *mesh);
    static Texture2D *getTexture(aiMaterial *material, aiTextureType type, const string& directory, map<string, Texture2D *> *loadedTextures);

    VertexArray *vertexArray{};

    Texture2D *diffuseMap{};
    Texture2D *specularMap{};
    Texture2D *normalMap{};
    Texture2D *heightMap{};

};


#endif //INC_KRYNE_ENGINE_MODELMESH_H
