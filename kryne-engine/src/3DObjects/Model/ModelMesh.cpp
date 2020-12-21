//
// Created by max on 29/07/2019.
//

#include "kryne-engine/3DObjects/Model/ModelMesh.h"

ModelMesh::ModelMesh(aiMesh *mesh, const aiScene *scene, const string& directory, map<string, Texture2D *> *loadedTextures)
{
    retrieveGeometry(mesh);

    if (scene->mMaterials) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        diffuseMap = getTexture(material, aiTextureType_DIFFUSE, directory, loadedTextures);
        specularMap = getTexture(material, aiTextureType_SPECULAR, directory, loadedTextures);
        normalMap = getTexture(material, aiTextureType_NORMALS, directory, loadedTextures);
        heightMap = getTexture(material, aiTextureType_HEIGHT, directory, loadedTextures);
    }
}


void ModelMesh::retrieveGeometry(aiMesh *mesh)
{
    vector<glm::vec3> vertices, normals, tangents, biTangents;
    vector<glm::vec2> textureCoordinates;
    vector<uint32_t> faces;

    for(uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertices.push_back(vector);

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        normals.push_back(vector);

        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            textureCoordinates.push_back(vec);
        }
        else
            textureCoordinates.emplace_back();

        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        tangents.push_back(vector);

        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        biTangents.push_back(vector);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            faces.push_back(face.mIndices[j]);
    }

    this->vertexArray = new VertexArray(&vertices);
    this->vertexArray->add_array(&normals);
    this->vertexArray->add_array(&textureCoordinates);
    this->vertexArray->add_array(&tangents);
    this->vertexArray->add_array(&biTangents);
    this->vertexArray->setUpElements(&faces, 1);
}


Texture2D *ModelMesh::getTexture(aiMaterial *material, aiTextureType type, const string& directory, map<string, Texture2D *> *loadedTextures)
{
    for (uint32_t i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);

        string fileName = str.C_Str();

        auto location = loadedTextures->find(fileName);
        if (location != loadedTextures->end())
            return location->second;

        auto texture = Texture2D::loadFromFileSync(directory + "/" + location->first).get();
        loadedTextures->insert(pair<string, Texture2D *>(fileName, texture));
        return texture;
    }

    return nullptr;
}


VertexArray *ModelMesh::getVertexArray() const {
    return vertexArray;
}


Texture2D *ModelMesh::getDiffuseMap() const {
    return diffuseMap;
}


Texture2D *ModelMesh::getSpecularMap() const {
    return specularMap;
}


Texture2D *ModelMesh::getNormalMap() const {
    return normalMap;
}


Texture2D *ModelMesh::getHeightMap() const {
    return heightMap;
}

