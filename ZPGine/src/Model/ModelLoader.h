#pragma once 

#include "Model.h"
#include "Material/Material.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/vector3.h>

namespace ZPG {

// Works as a SCOPED loader, holds data while loading in the model.
// MUST be discarded afterwards.
class ModelLoader {
public:
    ModelLoader(const std::string& path);
    Ref<Model> Load();

private:
    void TraverseNode(const aiScene* scene, const aiNode* node, const glm::mat4& parentTransform);
    void ProcessMesh(const aiScene* scene, const aiMesh* mesh, const glm::mat4& meshTransform);
    Ref<Material> ProcessMaterial(const aiScene* scene, const aiMaterial* material);

private:
    std::string m_Path;
    std::unordered_map<u32, Ref<Material>> m_Materials;
    std::vector<Ref<Mesh>> m_Meshes;
    u32 m_LoadingFlags;
};

}
