#include "ModelLoader.h"

#include "Resource/ResourceManager.h"
#include "Resource/CommonResources.h"
#include "Texture/Texture.h"
#include "Material/Material.h"
#include <assimp/postprocess.h>
#include <assimp/types.h>

#include "Mesh.h"
#include "Model.h"
#include "Vertex.h"
#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

static glm::mat4 AssimpToGLM(const aiMatrix4x4t<float>& m) {
    ZPG_PROFILE_FUNCTION();
    glm::mat4 ret(1.0f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[i][j] = m[i][j];
        }
    }
    return ret;
}

static v2 AssimpToGLM(const aiVector2D& v) {
    ZPG_PROFILE_FUNCTION();
    return v2(v.x, v.y);
}

static v3 AssimpToGLM(const aiVector3D& v) {
    ZPG_PROFILE_FUNCTION();
    return v3(v.x, v.y, v.z);
}

static v3 AssimpToGLM(const aiColor3D& c) {
    ZPG_PROFILE_FUNCTION();
    return v3(c.r, c.g, c.b);
}

static v4 AssimpToGLM(const aiColor4D& c) {
    ZPG_PROFILE_FUNCTION();
    return v4(c.r, c.g, c.b, c.a);
}


ModelLoader::ModelLoader(const std::string& path)
: m_Path(path) {
    ZPG_PROFILE_FUNCTION();
    m_LoadingFlags = aiProcess_Triangulate       // guarantees three vertices per face
                   | aiProcess_CalcTangentSpace  // guarantees normals and tangents exist
               //  | aiProcess_FlipUVs
                   ;
}

ref<Model> ModelLoader::Load() {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_DEBUG("Loading in model: {} ...", m_Path);
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_Path, m_LoadingFlags);
    ZPG_CORE_ASSERT(!(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode),
        "Model loading failed: {}", m_Path);

    // load the materials first (meshes refer to them by index, link them together right away)
    for (int i = 0; i < scene->mNumMaterials; i++) {
        m_Materials[i] = ProcessMaterial(scene, scene->mMaterials[i]);
    }

    // load in meshes (geometry) and reference the loaded material
    glm::mat4 rootTransform = AssimpToGLM(scene->mRootNode->mTransformation);
    TraverseNode(scene, scene->mRootNode, rootTransform);

    ref<Model> model = MakeRef(new Model(m_Meshes));
    return model;
}

ref<Texture> ModelLoader::LoadTexture(std::string const& textureFile) {
    ZPG_PROFILE_FUNCTION();
    std::string texturePath = std::filesystem::path(m_Path).parent_path() / textureFile;
    return Texture::Create(texturePath);
}

ref<Material> ModelLoader::ProcessMaterial(const aiScene* scene, const aiMaterial* material) {
    ZPG_PROFILE_FUNCTION();
    ResourceManager& res = ResourceManager::GetGlobal();

    ref<Material> myMaterial = MakeRef<Material>();


    // TODO: Determine if its PBR or Legacy workflow.
    myMaterial->SetShaderProgram(res.GetShaderProgram(CommonResources::SHADER_PROGRAM_DEFAULT));
    // myMaterial->SetShaderProgram(res.GetShaderProgram(CommonResources::SHADER_PROGRAM_PBR_PHONG));


    // get base color factor
    // first base color (PBR), fallback to diffuse (legacy)
    aiColor4D albedo(1.0, 1.0, 1.0, 1.0);

    if (material->Get(AI_MATKEY_BASE_COLOR, albedo) == AI_SUCCESS) {
        myMaterial->SetAlbedo(AssimpToGLM(albedo));
    } else if (material->Get(AI_MATKEY_COLOR_DIFFUSE, albedo) == AI_SUCCESS) {
        myMaterial->SetAlbedo(AssimpToGLM(albedo));
    } else {
        myMaterial->SetAlbedo(AssimpToGLM(albedo));
    }

    // get reflective factor
    // first metallic then specular
    float metallic = 0.5;

    if (material->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS) {
        myMaterial->SetMetallic(metallic);
    } else if (material->Get(AI_MATKEY_COLOR_SPECULAR, metallic) == AI_SUCCESS) {
        myMaterial->SetMetallic(metallic);
    } else {
        myMaterial->SetMetallic(metallic);
    }

    // get micro-surface factor
    // first roughness then glossiness, then 
    float roughness = 0.5;  

    if (material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS) {
        myMaterial->SetRoughness(roughness);
    } else if (material->Get(AI_MATKEY_SHININESS, roughness) == AI_SUCCESS) {
        // convert gloss to roughness
        roughness = 1.0f - glm::clamp(roughness / 128.0f, 0.0f, 1.0f);
        myMaterial->SetRoughness(roughness);
    } else {
        myMaterial->SetRoughness(roughness);
    }

    // set emissive
    aiColor3D emissive(0.0, 0.0, 0.0);

    if (material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) == AI_SUCCESS) {
        myMaterial->SetEmissive(v4(AssimpToGLM(emissive), 1.0));
    }



    // load base color map
    aiString textureFile;
    if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &textureFile) == AI_SUCCESS) {
        myMaterial->SetAlbedoMap(LoadTexture(textureFile.C_Str()));
    } else if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFile) == AI_SUCCESS) {
        myMaterial->SetAlbedoMap(LoadTexture(textureFile.C_Str()));
    } else {
        myMaterial->SetAlbedoMap(res.GetTexture(CommonResources::NULL_ALBEDO_MAP));
    }

    // load normal map
    if (material->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &textureFile) == AI_SUCCESS) {
        myMaterial->SetNormalMap(LoadTexture(textureFile.C_Str()));
    } else if (material->GetTexture(aiTextureType_NORMALS, 0, &textureFile) == AI_SUCCESS) {
        myMaterial->SetNormalMap(LoadTexture(textureFile.C_Str()));
    } else {
        myMaterial->SetNormalMap(res.GetTexture(CommonResources::NULL_NORMAL_MAP));
    }

    // load metalness map
    if (material->GetTexture(aiTextureType_METALNESS, 0, &textureFile) == AI_SUCCESS) {
        myMaterial->SetMetalnessMap(LoadTexture(textureFile.C_Str()));
    } else if (material->GetTexture(aiTextureType_SPECULAR, 0, &textureFile) == AI_SUCCESS) {
        myMaterial->SetMetalnessMap(LoadTexture(textureFile.C_Str()));
    } else {
        myMaterial->SetMetalnessMap(res.GetTexture(CommonResources::NULL_METALNESS_MAP));
    }

    // load roughness map
    if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &textureFile) == AI_SUCCESS) {
        myMaterial->SetRoughnessMap(LoadTexture(textureFile.C_Str()));
    } else if (material->GetTexture(aiTextureType_SHININESS, 0, &textureFile) == AI_SUCCESS) {
        myMaterial->SetRoughnessMap(LoadTexture(textureFile.C_Str()));
    } else {
        myMaterial->SetRoughnessMap(res.GetTexture(CommonResources::NULL_ROUGHNESS_MAP));
    }

    // load emissive map
    if (material->GetTexture(aiTextureType_EMISSIVE, 0, &textureFile) == AI_SUCCESS) {
        myMaterial->SetEmissiveMap(LoadTexture(textureFile.C_Str()));
    } else {
        myMaterial->SetEmissiveMap(res.GetTexture(CommonResources::NULL_EMISSIVE_MAP));
    }


    // load opacity
    // TODO: Get info about materials opacity.
    // This material is then rendered in the forward shading pass.
    f32 opacity = 1.0;
    aiString alphaMode;
    f32 transFactor = 1.0;

    if (
        material->Get(AI_MATKEY_BLEND_FUNC, alphaMode) == AI_SUCCESS && alphaMode == aiString("BLEND") ||
        material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS && opacity < 1.0 ||
        material->Get(AI_MATKEY_TRANSPARENCYFACTOR, transFactor) == AI_SUCCESS
    ) {
        ZPG_CORE_INFO("Model {} is transparent", m_Path);
        ZPG_CORE_INFO("Opacity (AI_MATKEY_OPACITY): {}, Transparency Factor: {}", opacity, transFactor);
    }

    return myMaterial;
}

void ModelLoader::TraverseNode(
    const aiScene* scene, 
    const aiNode* node, 
    const glm::mat4& parentTransform
) {
    ZPG_PROFILE_FUNCTION();
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
    ZPG_PROFILE_FUNCTION();
    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    // add vertices
    for (int i = 0; i < mesh->mNumVertices; i++) {
        v3 position(0.0f);
        v3 normal(0.0, 0.0, 1.0);
        v2 texCoord(0.0f);
        v3 tangent(1.0, 0.0, 0.0);

        position = AssimpToGLM(mesh->mVertices[i]);
        
        if (mesh->HasNormals()) {
            normal = AssimpToGLM(mesh->mNormals[i]);
        }

        if (mesh->HasTextureCoords(0)) {
            texCoord = AssimpToGLM(mesh->mTextureCoords[0][i]);
        }

        if (mesh->HasTangentsAndBitangents()) {
            tangent = AssimpToGLM(mesh->mTangents[i]);
        }

        vertices.push_back(Vertex(position, normal, texCoord, tangent));
    }

    // add indices
    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]); 
        }
    }
    
    // get material by index, assumes that the materials are loaded already
    ref<Material> material = m_Materials[mesh->mMaterialIndex];

    ref<Mesh> myMesh = Mesh::Create(vertices, indices, meshTransform);
    myMesh->SetMaterial(material);

    // add the processed mesh
    m_Meshes.push_back(myMesh); 
}

}
