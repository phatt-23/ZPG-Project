#pragma once 

#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/vector3.h>

namespace ZPG {

// scoped loader, holds data while loading in the model
// and is discarded afterwards
class ModelLoader {
public:
    Ref<Model> Load(const std::string& path);

private:
    void TraverseNode(const aiScene* scene, const aiNode* node, const glm::mat4& parentTransform);
    void ProcessMesh(const aiScene* scene, const aiMesh* mesh, const glm::mat4& meshTransform);

private:
    std::vector<Ref<Mesh>> m_Meshes;
    u32 m_LoadingFlags = aiProcess_Triangulate | aiProcess_FlipUVs;
};

}
