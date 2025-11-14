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
#include "Model/ModelLoader.h"
#include "Profiling/Instrumentor.h"
#include "Shader/ShaderDataType.h"
#include "Shader/Shader.h"
#include "Texture/TextureCubeMap.h"
#include "Texture/Texture2D.h"

namespace ZPG {

constexpr static f32 skyboxVertices[] = {
    // position
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
};

constexpr static u32 skyboxIndices[] = {
    0, 1, 2,   0, 2, 3, // Front face
    0, 4, 7,   0, 7, 3, // Left face
    3, 7, 6,   3, 6, 2, // Right face
    2, 6, 5,   2, 5, 1, // Back face
    1, 5, 4,   1, 4, 0, // Bottom face
    4, 5, 6,   4, 6, 7, // Top face
};

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
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Instance == nullptr, "The global resource manager was already instantiated.");
    s_Instance = MakeRef<ResourceManager>();

    // low-level
    InitDefaultShaderPrograms();
    InitDefaultVAOs();
    InitDefaultTextures();

    // high-level
    InitDefaultMaterials();
    InitDefaultMeshes();
    InitDefaultModels();
}

void ResourceManager::InitDefaultModels() {
    ZPG_PROFILE_FUNCTION();
    ref<Model> sphereModel = ModelLoader("./assets/models/sphere/scene.gltf").Load();
    s_Instance->AddModel(CommonResources::MODEL_SPHERE, sphereModel);
    s_Instance->AddMesh(CommonResources::MESH_SPHERE, sphereModel->GetMeshes().front());
    s_Instance->AddVAO(CommonResources::VAO_SPHERE, sphereModel->GetMeshes().front()->GetVertexArray());

    ref<Model> boxModel = Model::Create({ s_Instance->GetMesh(CommonResources::MESH_BOX) });
    s_Instance->AddModel(CommonResources::MODEL_BOX, boxModel);

    ref<Model> skydomeModel = ModelLoader("./assets/models/skydome/skydome.obj").Load();
    s_Instance->AddModel(CommonResources::MODEL_SKYDOME, skydomeModel);
    s_Instance->AddMesh(CommonResources::MESH_SKYDOME, skydomeModel->GetMeshes().front());
    s_Instance->AddVAO(CommonResources::VAO_SKYDOME, skydomeModel->GetMeshes().front()->GetVertexArray());

    ref<Model> skyboxModel = ModelLoader("./assets/models/skybox/skybox.obj").Load();
    s_Instance->AddModel(CommonResources::MODEL_SKYBOX, skyboxModel);
    s_Instance->AddMesh(CommonResources::MESH_SKYBOX, skyboxModel->GetMeshes().front());
    s_Instance->AddVAO(CommonResources::VAO_SKYBOX, skyboxModel->GetMeshes().front()->GetVertexArray());
}

void ResourceManager::InitDefaultShaderPrograms() {
    ZPG_PROFILE_FUNCTION();
    using namespace std;
    using Stages = vector<string>;
    using Route = string;

    filesystem::path basePath = "./assets/shaders/SSBO/";

    map<Route, variant< Stages, Route >> shaderProgramConfig = {
        {CommonResources::SHADER_PROGRAM_PBR,           Stages{ "vertex/General.vert", "fragment/PBR.frag"         }},
        {CommonResources::SHADER_PROGRAM_CONSTANT,      Stages{ "vertex/General.vert", "fragment/Constant.frag"    }},
        {CommonResources::SHADER_PROGRAM_LAMBERT,       Stages{ "vertex/General.vert", "fragment/Lambert.frag"     }},
        {CommonResources::SHADER_PROGRAM_PHONG,         Stages{ "vertex/General.vert", "fragment/Phong.frag"       }},
        {CommonResources::SHADER_PROGRAM_BLINN_PHONG,   Stages{ "vertex/General.vert", "fragment/Blinn-Phong.frag" }},
        {CommonResources::SHADER_PROGRAM_SKYBOX,        Stages{ "vertex/Skybox.vert", "fragment/Skybox.frag" }},
        {CommonResources::SHADER_PROGRAM_SKYDOME,       Stages{ "vertex/Skydome.vert", "fragment/Skydome.frag" }},
        {CommonResources::SHADER_PROGRAM_DEFAULT,       Route(CommonResources::SHADER_PROGRAM_BLINN_PHONG) },
    };


    for (auto& [name, entry] : shaderProgramConfig) {
        // Load shaders from files
        if (std::vector<string>* stages = get_if<Stages>(&entry)) {
            for (auto& stage : *stages)
                stage = (basePath / stage).string();

            s_Instance->LoadShaderProgram(name, *stages);
        }

        // Reuse existing program
        else if (Route* route = get_if<Route>(&entry)) {
            s_Instance->AddShaderProgram(name, s_Instance->GetShaderProgram(*route));
        }
    }
}

void ResourceManager::InitDefaultTextures() {
    ZPG_PROFILE_FUNCTION();
    // these data must be passed in as array of bytes
    // not as a single number, because my CPU stores
    // it in little endian


    std::unordered_map<const char*, std::vector<u8>> nullMapData = {
        {CommonResources::NULL_ALBEDO_MAP, { 255, 255, 255, 255 }},
        {CommonResources::NULL_NORMAL_MAP, { 127, 127, 255, 255 }},
        {CommonResources::NULL_METALNESS_MAP, { 255, 255, 255, 255 }},
        {CommonResources::NULL_ROUGHNESS_MAP, { 255, 255, 255, 255 }},
        {CommonResources::NULL_EMISSIVE_MAP, { 255, 255, 255, 255 }},
        {CommonResources::NULL_SKYDOME_MAP, { 0, 0, 0, 0 }},
    };

    for (auto& [name, bytes] : nullMapData) {
        auto nullMap = Texture2D::Create(name, 1, 1, TextureDataFormat::RGBA8);

        nullMap->SetData(bytes.data(), sizeof(u8) * bytes.size());

        s_Instance->AddTexture(nullMap->GetName(), nullMap);
    }

    ref<TextureCubeMap> cubemapTexture = TextureCubeMap::Create(CommonResources::NULL_CUBEMAP, 1, TextureDataFormat::RGBA8);

    for (int i = 0; i < 6; i++)
    {
        u8 nullCubemapData[] = { 0, 0, 0, 0 };
        cubemapTexture->SetFaceData((CubemapFace)i, nullCubemapData, sizeof(nullCubemapData));
    }

    s_Instance->AddTexture(CommonResources::NULL_CUBEMAP, cubemapTexture);
}

void ResourceManager::InitDefaultMaterials() {
    ZPG_PROFILE_FUNCTION();
    // Null Material
    s_Instance->AddMaterial(CommonResources::NULL_MATERIAL, MakeRef(new Material()));
}


void ResourceManager::InitDefaultVAOs() {
    ZPG_PROFILE_FUNCTION();
    BufferLayout phattLayout = {
        {ShaderDataType::Float3, "a_Pos"},
        {ShaderDataType::Float3, "a_Normal"},
        {ShaderDataType::Float2, "a_TexCoord"},
    };

    s_Instance->AddVAO(
        CommonResources::VAO_BOX,
        VertexArray::Create(
            { VertexBuffer::Create(phatt::boxVertices, sizeof(phatt::boxVertices), phattLayout) },
            IndexBuffer::Create(phatt::boxIndices, ZPG_ARRAY_LENGTH(phatt::boxIndices))
        )
    );
}

void ResourceManager::InitDefaultMeshes() {
    ZPG_PROFILE_FUNCTION();
    s_Instance->AddMesh(
        CommonResources::MESH_BOX,
        Mesh::Create(s_Instance->GetVAO(CommonResources::VAO_BOX)));
}

void ResourceManager::Shutdown() {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Instance != nullptr, "The resource manager that wasn't initialized can't be shutdown.");
    // delete s_Instance;
    // s_Instance = nullptr;
}

ResourceManager& ResourceManager::GetGlobal() {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Instance != nullptr, "The resource manager that wasn't initialized");
    return *s_Instance;
}

ref<ResourceManager> ResourceManager::GetGlobalRef() {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Instance != nullptr, "The resource manager that wasn't initialized");
    return s_Instance;
}


void ResourceManager::LoadModel(const std::string& name, const std::string& path) {
    ZPG_PROFILE_FUNCTION();
    m_ModelLib.LoadModel(name, path);
}
void ResourceManager::AddModel(const std::string& name, const ref<Model>& model) {
    ZPG_PROFILE_FUNCTION();
    m_ModelLib.AddModel(name, model);
}
const std::unordered_map<std::string, ref<Model>>& ResourceManager::GetModels() const {
    ZPG_PROFILE_FUNCTION();
    return m_ModelLib.GetModels();
}
const ref<Model>& ResourceManager::GetModel(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    return m_ModelLib.GetModel(name);
}
bool ResourceManager::HasModel(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    return m_ModelLib.Exists(name);
}



void ResourceManager::LoadShaderProgram(
    const std::string& name,
    const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath
)
{
    ZPG_PROFILE_FUNCTION();

    auto program = ShaderProgram::Create(name, {
        Shader::Create(vertexShaderPath),
        Shader::Create(fragmentShaderPath),
    });
    m_ShaderProgramLib.AddShaderProgram(name, program);
}

void ResourceManager::LoadShaderProgram(const std::string& name, const std::vector<std::string>& stages)
{
    ZPG_PROFILE_FUNCTION();
    std::vector<ref<Shader>> shaders;
    shaders.reserve(stages.size());

    for (auto& stage : stages) {
        shaders.push_back(Shader::Create(stage));
    }

    auto program = ShaderProgram::Create(name, shaders);

    m_ShaderProgramLib.AddShaderProgram(name, program);
}

void ResourceManager::AddShaderProgram(const std::string& name, const ref<ShaderProgram>& shaderProgram)
{
    ZPG_PROFILE_FUNCTION();
    m_ShaderProgramLib.AddShaderProgram(name, shaderProgram);
}

const ref<ShaderProgram>& ResourceManager::GetShaderProgram(const std::string& name)
{
    ZPG_PROFILE_FUNCTION();
    return m_ShaderProgramLib.GetShaderProgram(name);
}

const ResourceManager::MapOf<std::shared_ptr<ShaderProgram>>& ResourceManager::GetShaderPrograms() const
{
    ZPG_PROFILE_FUNCTION();
    return m_ShaderProgramLib.GetShaders();
}

bool ResourceManager::HasShaderProgram(const std::string& name) const
{
    ZPG_PROFILE_FUNCTION();
    return m_ShaderProgramLib.Exists(name);
}



void ResourceManager::LoadTexture(const std::string& name, const std::string& path)
{
    ZPG_PROFILE_FUNCTION();
    ref<Texture2D> texture = Texture2D::Create(path);
    m_TextureLib.AddTexture(name, texture);
}
void ResourceManager::AddTexture(const std::string& name, const ref<Texture>& texture)
{
    ZPG_PROFILE_FUNCTION();
    m_TextureLib.AddTexture(name, texture);
}
const ref<Texture>& ResourceManager::GetTexture(const std::string& name)
{
    ZPG_PROFILE_FUNCTION();
    return m_TextureLib.GetTexture(name);
}

const ResourceManager::MapOf<ref<Texture>>& ResourceManager::GetTextures() const {
    ZPG_PROFILE_FUNCTION();
    return m_TextureLib.GetTextures();
}

bool ResourceManager::HasTexture(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    return m_TextureLib.Exists(name);
}

void ResourceManager::AddVAO(const std::string& name, const ref<VertexArray>& vao) {
    ZPG_PROFILE_FUNCTION();
    m_VAOLib.AddVAO(name, vao);
}

const ref<VertexArray>& ResourceManager::GetVAO(const std::string& name) {
    ZPG_PROFILE_FUNCTION();
    return m_VAOLib.GetVAO(name);
}

const ResourceManager::MapOf<std::shared_ptr<VertexArray>>& ResourceManager::GetVAOs() const {
    ZPG_PROFILE_FUNCTION();
    return m_VAOLib.GetVAOs();
}

bool ResourceManager::HasVAO(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    return m_VAOLib.Exists(name);
}


void ResourceManager::AddMaterial(const std::string& name, const ref<Material>& material) {
    ZPG_PROFILE_FUNCTION();
    m_MaterialLib.AddMaterial(name, material);
}

ref<Material> ResourceManager::GetMaterial(const std::string& name) {
    ZPG_PROFILE_FUNCTION();
    return m_MaterialLib.GetMaterial(name);
}
const std::unordered_map<std::string, ref<Material>>&
ResourceManager::GetMaterials(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    return m_MaterialLib.GetMaterials();
}
bool ResourceManager::HasMaterial(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    return m_MaterialLib.Exists(name);
}

void ResourceManager::AddMesh(const std::string& name, const ref<Mesh>& mesh) {
    ZPG_PROFILE_FUNCTION();
    m_MeshLib.AddMesh(name, mesh);
}

const ref<Mesh>& ResourceManager::GetMesh(const std::string& name) {
    ZPG_PROFILE_FUNCTION();
    return m_MeshLib.GetMesh(name);
}

const ResourceManager::MapOf<std::shared_ptr<Mesh>>& ResourceManager::GetMeshes() const {
    ZPG_PROFILE_FUNCTION();
    return m_MeshLib.GetMeshes();
}

bool ResourceManager::HasMesh(const std::string& name) {
    ZPG_PROFILE_FUNCTION();
    return m_MeshLib.Exists(name);
}

}
