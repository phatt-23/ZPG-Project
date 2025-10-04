#include "ModelLoader.h"
#include "Core/Core.h"
#include <glm/gtc/type_ptr.hpp>

namespace ZPG {

#define ZPG_ASSIMP_READ_FILE_PROCESS_FLAGS (aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenNormals)


Ref<Model> ModelLoader::LoadModel(const std::string& path) {
    ZPG_NOT_IMPL();
    // Assimp::Importer importer;
    //
    // const aiScene* scene = importer.ReadFile(path, ZPG_ASSIMP_READ_FILE_PROCESS_FLAGS);
    //
    // if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    //     ZPG_UNREACHABLE("Assimp importer failed to read the file: {}", path);
    // }
    //
    // Ref<Model> model = CreateRef<Model>();
    //
    // ProcessNode(scene->mRootNode, scene, model->GetRootNode());
    //
    // return model;
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Ref<ModelNode>& parent) {
    ZPG_NOT_IMPL();
    // aiMatrix4x4 aiTransform = node->mTransformation;
    // glm::mat4 local = glm::transpose(glm::make_mat4(&aiTransform.a1));
    //
    // ModelNode* newNode = parent->AddChild(CreateRef<ModelNode>())
}
Mesh* ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    ZPG_NOT_IMPL();
    // std::vector<Vertex> vertices;
    // std::vector<u32> indices;
    //
    // for (size_t i = 0; i < mesh->m)
}
Material* ModelLoader::ProcessMaterial(aiMaterial* mat) {
    ZPG_NOT_IMPL();
}

}

