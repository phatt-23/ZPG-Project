#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include "Model/Material.h"

namespace ZPG {

class ModelLoader {
public:
    static Ref<Model> LoadModel(const std::string& path);
private:
    static void ProcessNode(aiNode* node, const aiScene* scene, Ref<ModelNode>& parent);
    static Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    static Material* ProcessMaterial(aiMaterial* mat);
};

}
