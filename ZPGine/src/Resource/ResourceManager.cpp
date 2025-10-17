#include "ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "CommonResources.h"
#include "../../../assets/models/nemec/sphere.h"
#include "../../../assets/models/phatt/box.h"
#include "Debug/Asserter.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Buffer/VertexArray.h"
#include "Buffer/BufferLayout.h"
#include "Shader/ShaderDataType.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"

namespace ZPG {

ResourceManager::ResourceManager() 
: m_ModelLib(), m_ShaderProgramLib() {

}
ResourceManager::~ResourceManager() {

}

/**
 * Material system expects these resources:
 * - Null AlbedoMap
 * - Null NormalMap
 * - DefaultLit shader program
 *
 * Mesh system expects these resources:
 * - Null Material
 */
void ResourceManager::Init() {
    ZPG_CORE_ASSERT(s_Instance == nullptr, "The global resource manager was already instantiated.");
    s_Instance = MakeRef<ResourceManager>();

    InitDefaultTextures();
    InitDefaultShaderPrograms();
    InitDefaultMaterials();
    InitDefaultModels();
}

void ResourceManager::InitDefaultModels() {
    // TODO: make a mesh library or even VAO library and put these there
    //
    ref<Model> sphereModel = Model::Create({
        Mesh::Create(
            VertexArray::Create({
                VertexBuffer::Create(nemec::sphere, sizeof(nemec::sphere), {
                    {ShaderDataType::Float3, "a_Pos"},
                    {ShaderDataType::Float3, "a_Normal"},
                })
            })
        )
    });

    s_Instance->AddModel(CommonResources::MODEL_SPHERE, sphereModel);

    ref<Model> boxModel = Model::Create({
        Mesh::Create(
            VertexArray::Create({
                VertexBuffer::Create(phatt::boxVertices, sizeof(phatt::boxVertices), {
                    {ShaderDataType::Float3, "a_Pos"},
                    {ShaderDataType::Float3, "a_Normal"},
                    {ShaderDataType::Float2, "a_TexCoord"},
                })
            }, IndexBuffer::Create(phatt::boxIndices, ZPG_ARRAY_LENGTH(phatt::boxIndices)))
        )
    });

    s_Instance->AddModel(CommonResources::MODEL_BOX, boxModel);
}

void ResourceManager::InitDefaultShaderPrograms() {
    s_Instance->LoadShaderProgram(
        CommonResources::SHADER_PROGRAM_DEFAULT_LIT,
        "./assets/shaders/vertex/DefaultLit.vert",
        "./assets/shaders/fragment/DefaultLit.frag");

    s_Instance->LoadShaderProgram(
        CommonResources::SHADER_PROGRAM_PBR,
        "./assets/shaders/vertex/PBR.vert",
        "./assets/shaders/fragment/PBR.frag");

    s_Instance->LoadShaderProgram(
        CommonResources::SHADER_PROGRAM_CONSTANT,
        "./assets/shaders/vertex/Constant.vert",
        "./assets/shaders/fragment/Constant.frag");

    s_Instance->LoadShaderProgram(
        CommonResources::SHADER_PROGRAM_LAMBERT,
        "./assets/shaders/vertex/Lambert.vert",
        "./assets/shaders/fragment/Lambert.frag");

    s_Instance->LoadShaderProgram(
        CommonResources::SHADER_PROGRAM_PHONG,
        "./assets/shaders/vertex/Phong.vert",
        "./assets/shaders/fragment/Phong.frag");

    s_Instance->LoadShaderProgram(
        CommonResources::SHADER_PROGRAM_BLINN_PHONG,
        "./assets/shaders/vertex/Blinn-Phong.vert",
        "./assets/shaders/fragment/Blinn-Phong.frag");
}

void ResourceManager::InitDefaultTextures() {
    // Null Albedo map
    ref<Texture> albedoMapNull = Texture::Create(CommonResources::NULL_ALBEDO_MAP, 1, 1);
    u32 nullAlbedoData = 0xFFFFFF;
    albedoMapNull->SetData(&nullAlbedoData, sizeof(nullAlbedoData));
    s_Instance->AddTexture(albedoMapNull->GetName(), albedoMapNull);

    // Normal Map
    ref<Texture> normalMapNull = Texture::Create(CommonResources::NULL_NORMAL_MAP, 1, 1);
    u32 nullNormalData = 0x7F7FFF; // (0.5, 0.5, 1.0) -> (0.0, 0.0, 1.0) points in z direction
    normalMapNull->SetData(&nullNormalData, sizeof(nullNormalData));
    s_Instance->AddTexture(normalMapNull->GetName(), normalMapNull);

    // Metalness Map
    ref<Texture> metalnessMapNull = Texture::Create(CommonResources::NULL_METALNESS_MAP, 1, 1);
    u32 nullMetalnessData = 0xFFFFFFFF;
    metalnessMapNull->SetData(&nullMetalnessData, sizeof(nullMetalnessData));
    s_Instance->AddTexture(metalnessMapNull->GetName(), metalnessMapNull);

    // Roughness Map
    ref<Texture> nullRoughnessMap = Texture::Create(CommonResources::NULL_ROUGHNESS_MAP, 1, 1);
    u32 nullRoughnessData = 0xFFFFFFFF;
    metalnessMapNull->SetData(&nullRoughnessData, sizeof(nullRoughnessData));
    s_Instance->AddTexture(nullRoughnessMap->GetName(), nullRoughnessMap);
}

void ResourceManager::InitDefaultMaterials() {
    // Null Material
    s_Instance->AddMaterial(CommonResources::NULL_MATERIAL, MakeRef(new Material()));
}

void ResourceManager::Shutdown() {
    ZPG_CORE_ASSERT(s_Instance != nullptr, "The resource manager that wasn't initialized can't be shutdown.");
    // delete s_Instance;
    // s_Instance = nullptr;
}



void ResourceManager::LoadModel(const std::string& name, const std::string& path) {
    m_ModelLib.LoadModel(name, path);
}
void ResourceManager::AddModel(const std::string& name, const ref<Model>& model) {
    m_ModelLib.AddModel(name, model);
}
const ref<Model>& ResourceManager::GetModel(const std::string& name) const {
    return m_ModelLib.GetModel(name);
}
bool ResourceManager::HasModel(const std::string& name) const {
    return m_ModelLib.Exists(name);
}



void ResourceManager::LoadShaderProgram(
    const std::string& name, 
    const std::string& vertexShaderPath, 
    const std::string& fragmentShaderPath) {
    
    auto program = ShaderProgram::Create(name, {
        Shader::Create(vertexShaderPath),
        Shader::Create(fragmentShaderPath),
    });
    m_ShaderProgramLib.AddShaderProgram(name, program);
}

void ResourceManager::AddShaderProgram(const std::string& name, const ref<ShaderProgram>& shaderProgram) {
    m_ShaderProgramLib.AddShaderProgram(name, shaderProgram);
}
    
const ref<ShaderProgram>& ResourceManager::GetShaderProgram(const std::string& name) {
    return m_ShaderProgramLib.GetShaderProgram(name);
}

bool ResourceManager::HasShaderProgram(const std::string& name) const {
    return m_ShaderProgramLib.Exists(name);
}



void ResourceManager::LoadTexture(const std::string& name, const std::string& path) {
    ref<Texture> texture = Texture::Create(path); 
    m_TextureLib.AddTexture(name, texture);
}
void ResourceManager::AddTexture(const std::string& name, const ref<Texture>& texture) {
    m_TextureLib.AddTexture(name, texture);
}
const ref<Texture>& ResourceManager::GetTexture(const std::string& name) {
    return m_TextureLib.GetTexture(name);
}
bool ResourceManager::HasTexture(const std::string& name) const {
    return m_TextureLib.Exists(name);
}



void ResourceManager::AddMaterial(const std::string& name, const ref<Material>& material) {
    m_MaterialLib.AddMaterial(name, material);
}

ref<Material> ResourceManager::GetMaterial(const std::string& name) {
    return m_MaterialLib.GetMaterial(name);
}

bool ResourceManager::HasMaterial(const std::string& name) const {
    return m_MaterialLib.Exists(name);
}

}
