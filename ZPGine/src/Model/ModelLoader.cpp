#include "ModelLoader.h"
#include "Core/ResourceManager.h"
#include <filesystem>
#include "Texture/Texture.h"

namespace ZPG {

static glm::mat4 AssimpToGLM(const aiMatrix4x4t<float>& m) {
    glm::mat4 ret(1.0f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[i][j] = m[i][j];
        }
    }
    return ret;
}

static glm::vec3 AssimpToGLM(const aiVector3D& v) {
    return glm::vec3(v.x, v.y, v.z);
}

static glm::vec3 AssimpToGLM(const aiColor3D& c) {
    return glm::vec3(c.r, c.g, c.b);
}

static glm::vec2 AssimpToGLM(const aiVector2D& v) {
    return glm::vec2(v.x, v.y);
}

ModelLoader::ModelLoader(const std::string& path) 
: m_Path(path) {
    m_LoadingFlags = aiProcess_Triangulate       // guarantees three vertices per face
                   | aiProcess_CalcTangentSpace  // guarantees normals exist
               //  | aiProcess_FlipUVs
                   ;
}

Ref<Model> ModelLoader::Load() {
    ZPG_CORE_DEBUG("Loading in model: {} ...", m_Path);
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_Path, m_LoadingFlags);
    ZPG_CORE_ASSERT(!(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode),
        "Model loading failed: {}", m_Path);

    // load the matrials first (meshes refer to them by index, link them together right away)
    for (int i = 0; i < scene->mNumMaterials; i++) {
        m_Materials[i] = ProcessMaterial(scene, scene->mMaterials[i]);
    }

    // load in meses (geometry) and reference the loaded material
    glm::mat4 rootTransform = AssimpToGLM(scene->mRootNode->mTransformation);
    TraverseNode(scene, scene->mRootNode, rootTransform);

    Ref<Model> model = CreateRef(new Model(m_Meshes));
    return model;
}

Ref<Material> ModelLoader::ProcessMaterial(const aiScene* scene, const aiMaterial* material) {
    Ref<Material> myMaterial = CreateRef<Material>();

    // TODO: This shouldn't be called like this.
    // It makes the transient model loader depend on global state.
    // For now it's ok.
    myMaterial->SetShaderProgram(ResourceManager::GetGlobal().GetShaderProgram("DefaultLit"));

    // load diffuse
    aiString textureFile;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFile) == AI_SUCCESS) {
        std::string texturePath = std::filesystem::path(m_Path).parent_path() / textureFile.C_Str();
        Ref<Texture> texture = Texture::Create(texturePath);
        myMaterial->SetAlbedoMap(texture);
    }

    // load normal map
    if (material->GetTexture(aiTextureType_NORMALS, 0, &textureFile) == AI_SUCCESS) {
        std::string texturePath = std::filesystem::path(m_Path).parent_path() / textureFile.C_Str();
        Ref<Texture> texture = Texture::Create(texturePath);
        myMaterial->SetNormalMap(texture);
    }

    // get color and shininess
    aiColor3D color; 
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        myMaterial->SetAlbedoColor(AssimpToGLM(color));
    }

    float shininess = 32.0f;  // default
    material->Get(AI_MATKEY_SHININESS, shininess);
    myMaterial->SetMetallic(shininess);
    
    float roughness = 32.0f;  // default
    material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
    myMaterial->SetRoughness(roughness);

    return myMaterial;
}

void ModelLoader::TraverseNode(
    const aiScene* scene, 
    const aiNode* node, 
    const glm::mat4& parentTransform
) {
    glm::mat4 localTransform = AssimpToGLM(node->mTransformation);
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

        position = AssimpToGLM(mesh->mVertices[i]);
        
        if (mesh->HasNormals()) {
            normal = AssimpToGLM(mesh->mNormals[i]);
        }

        if (mesh->HasTextureCoords(0)) {
            texCoord = AssimpToGLM(mesh->mTextureCoords[0][i]);
        }

        vertices.push_back(Vertex(position, normal, texCoord));
    }

    // add indices
    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]); 
        }
    }
    
    // get material by index, assumes that the materials are loaded already
    Ref<Material> material = m_Materials[mesh->mMaterialIndex];

    Ref<Mesh> myMesh = CreateRef<Mesh>(vertices, indices, meshTransform);
    myMesh->SetMaterial(material);

    // add the processed mesh
    m_Meshes.push_back(myMesh); 
}

}
