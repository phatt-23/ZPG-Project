#include "ModelLoader.h"
#include <fstream>

namespace ZPG {

static glm::mat4 AssimpMatrixToGlm(const aiMatrix4x4t<float>& m) {
    glm::mat4 ret(1.0f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[i][j] = m[i][j];
        }
    }
    return ret;
}

Ref<Model> ModelLoader::Load(const std::string& path) {
    ZPG_CORE_DEBUG("Loading in model: {}", path);
    
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, m_LoadingFlags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        ZPG_UNREACHABLE("Model loading failed: {}", path);
    }

    glm::mat4 rootTransform = AssimpMatrixToGlm(scene->mRootNode->mTransformation);
    TraverseNode(scene, scene->mRootNode, rootTransform);

    ZPG_CORE_DEBUG("Meshes count: {}", m_Meshes.size());

    Ref<Model> model = CreateRef(new Model(m_Meshes));
    
    m_Meshes.clear(); 
    return model;
}

void ModelLoader::TraverseNode(const aiScene* scene, const aiNode* node, const glm::mat4& parentTransform) {
    glm::mat4 localTransform = AssimpMatrixToGlm(node->mTransformation);
    glm::mat4 nodeTransform = parentTransform * localTransform;

    // process this node's meshes
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(scene, mesh, nodeTransform);
    }

    // traverse children
    for (int i = 0; i < node->mNumChildren; i++) {
        aiNode* child = node->mChildren[i];
        TraverseNode(scene, child, nodeTransform);
    }
}

void ModelLoader::ProcessMesh(const aiScene* scene, const aiMesh* mesh, const glm::mat4& meshTransform) {
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    // add vertices
    for (int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position(0.0f);
        glm::vec3 normal(0.0f);
        glm::vec2 texCoord(0.0f);

        aiVector3D p = mesh->mVertices[i];
        position.x = p.x;
        position.y = p.y;
        position.z = p.z;
        
        if (mesh->HasNormals()) {
            aiVector3D n = mesh->mNormals[i];
            normal.x = n.x;
            normal.y = n.y;
            normal.z = n.z;
        }

        if (mesh->HasTextureCoords(0)) {
            aiVector3D t = mesh->mTextureCoords[0][i];
            texCoord.x = t.x;
            texCoord.y = t.y;
        }

        Vertex vertex(
            glm::vec3(p.x, p.y, p.z),
            glm::vec3(normal.x, normal.y, normal.z),
            glm::vec2(texCoord.x, texCoord.y)
        );

        vertices.push_back(vertex);
    }

    // add indices
    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]); 
        }
    }
    
    // add the processed mesh
    m_Meshes.push_back( CreateRef<Mesh>(vertices, indices, meshTransform) ); 
}

}
