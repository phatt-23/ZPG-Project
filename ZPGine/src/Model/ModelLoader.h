#pragma once 

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/material.h>

namespace ZPG {

class Model;
class Mesh;
class Texture2D;
class Material;

// Works as a SCOPED loader, holds data while loading in the model.
// MUST be discarded afterward.
class ModelLoader {
public:
    ModelLoader(const std::string& path);
    ~ModelLoader();
    ref<Model> Load();

private:
    void TraverseNode(const aiScene* scene, const aiNode* node, const glm::mat4& parentTransform);
    void ProcessMesh(const aiScene* scene, const aiMesh* mesh, const glm::mat4& meshTransform);
    ref<Material> ProcessMaterial(const aiScene* scene, const aiMaterial* material);
    ref<Texture2D> LoadTexture(std::string const& textureFile);

private:
    std::string m_Path;
    std::unordered_map<u32, ref<Material>> m_Materials;
    std::vector<ref<Mesh>> m_Meshes;
    u32 m_LoadingFlags;
};

}
