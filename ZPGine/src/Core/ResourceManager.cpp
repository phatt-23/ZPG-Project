#include "ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>

namespace ZPG {

ResourceManager::ResourceManager() 
: m_ModelLib(), m_ShaderProgramLib() {

}
ResourceManager::~ResourceManager() {

}

void ResourceManager::Init() {
    ZPG_CORE_ASSERT(s_Instance == nullptr, "The global resource manager was already instantiated.");
    s_Instance = CreateRef<ResourceManager>();

    // Material system expects these resources:
    // Null AlbedoMap
    // Null NormalMap
    // DefaultLit shader program
    
    // Mesh system expects these resources:
    // Null Material

    // Default Lit shader program
    Ref<ShaderProgram> shaderProgram = ShaderProgram::Create("DefaultLit", {
        Shader::Create("./assets/shaders/vertex/DefaultLit.vert"),
        Shader::Create("./assets/shaders/fragment/DefaultLit.frag"),
    });
    s_Instance->AddShaderProgram(shaderProgram->GetName(), shaderProgram);

    // Null Albedo map
    Ref<Texture> albedoMapNull = Texture::Create("AlbedoMap:Null", 1, 1);
    u32 nullAlbedoData = 0xFFFFFF;
    albedoMapNull->SetData(&nullAlbedoData, sizeof(nullAlbedoData));
    s_Instance->AddTexture(albedoMapNull->GetName(), albedoMapNull);

    // Null Normal map
    Ref<Texture> normalMapNull = Texture::Create("NormalMap:Null", 1, 1);
    u32 nullNormalData = 0x7F7FFF;
    normalMapNull->SetData(&nullNormalData, sizeof(nullNormalData));
    s_Instance->AddTexture(normalMapNull->GetName(), normalMapNull);

    // Null Material
    s_Instance->AddMaterial("null", CreateRef(new Material()));
    
}

void ResourceManager::Shutdown() {
    ZPG_CORE_ASSERT(s_Instance != nullptr, "The resource manager that wasn't initialized can't be shutdown.");
    // delete s_Instance;
    // s_Instance = nullptr;
}

// ---------- model ---------------

void ResourceManager::LoadModel(const std::string& name, const std::string& path) {
    m_ModelLib.LoadModel(name, path);
}
void ResourceManager::AddModel(const std::string& name, const Ref<Model>& model) {
    m_ModelLib.AddModel(name, model);
}
const Ref<Model>& ResourceManager::GetModel(const std::string& name) const {
    return m_ModelLib.GetModel(name);
}
bool ResourceManager::HasModel(const std::string& name) const {
    return m_ModelLib.Exists(name);
}

// ---------- shader program ---------------

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

void ResourceManager::AddShaderProgram(const std::string& name, const Ref<ShaderProgram>& shaderProgram) {
    m_ShaderProgramLib.AddShaderProgram(name, shaderProgram);
}
    
const Ref<ShaderProgram>& ResourceManager::GetShaderProgram(const std::string& name) {
    return m_ShaderProgramLib.GetShaderProgram(name);
}

bool ResourceManager::HasShaderProgram(const std::string& name) const {
    return m_ShaderProgramLib.Exists(name);
}

// ---------- texture ---------------
void ResourceManager::LoadTexture(const std::string& name, const std::string& path) {
    Ref<Texture> texture = Texture::Create(path); 
    m_TextureLib.AddTexture(name, texture);
}
void ResourceManager::AddTexture(const std::string& name, const Ref<Texture>& texture) {
    m_TextureLib.AddTexture(name, texture);
}
const Ref<Texture>& ResourceManager::GetTexture(const std::string& name) {
    return m_TextureLib.GetTexture(name);
}
bool ResourceManager::HasTexture(const std::string& name) const {
    return m_TextureLib.Exists(name);
}

// --------- material ---------------

void ResourceManager::AddMaterial(const std::string& name, const Ref<Material>& material) {
    m_MaterialLib.AddMaterial(name, material);
}

const Ref<Material>& ResourceManager::GetMaterial(const std::string& name) {
    return m_MaterialLib.GetMaterial(name);
}

bool ResourceManager::HasMaterial(const std::string& name) const {
    return m_MaterialLib.Exists(name);
}

}
