#pragma once
#include "Material/MaterialLibrary.h"
#include "Model/ModelLibrary.h"
#include "Shader/ShaderProgramLibrary.h"
#include "Texture/TextureLibrary.h"
#include "Model/MeshLibrary.h"
#include "VertexArrayLibrary.h"

namespace ZPG {

class Model;
class Material;
class Mesh;

class ShaderProgram;
class Texture;
class VertexArray;


// Lifetime - whole duration of the application
//
// The resource manager holds all the shared assets that persist throughout the application run.
// It is implemented as a singleton, but it also allows the client to instantiate their own res manager
// and inject that into the scenes and layers, although by default, if nothing is passed (nullptr is default),
// then the global ResourceManager::Get() is 'injected' instead. The client doesn't care either way,
// the client code will look the same (m_ResManager.GetShaderProgram("Phong") could fetch the shader program from
// global resources or locally owned res manager).
class ResourceManager {
    template<typename T>
    using MapOf = std::unordered_map<std::string, T>;

public:
    ResourceManager();
    ~ResourceManager();

    // called from application
    static void Init();
    static void Shutdown();

    static ResourceManager& GetGlobal();
    static ref<ResourceManager> GetGlobalRef();

    /**
     * High-level: 
     *  - Models
     *  - Materials
     *  - Meshes
    */

    /**
     * Models
     */

    void LoadModel(const std::string& name, const std::string& path);
    void AddModel(const std::string& name, const ref<Model>& model);

    const ref<Model>& GetModel(const std::string& name) const;
    const MapOf<ref<Model>>& GetModels() const;

    bool HasModel(const std::string& name) const;


    /**
     * Materials
     */

    void AddMaterial(const std::string& name, const ref<Material>& material);

    ref<Material> GetMaterial(const std::string& name);
    const MapOf<ref<Material>>& GetMaterials(const std::string& name) const;

    bool HasMaterial(const std::string& name) const;

    /**
     * Meshes
     */

    void AddMesh(const std::string& name, const ref<Mesh>& mesh);

    const ref<Mesh>& GetMesh(const std::string& name);
    const MapOf<ref<Mesh>>& GetMeshes() const;

    bool HasMesh(const std::string& name);

    /**
     * Low-level:
     *  - ShaderPrograms
     *  - Textures
     *  - VAOs
     */
    
    /**
     * ShaderPrograms
     */
    void LoadShaderProgram(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void LoadShaderProgram(const std::string& name, const std::vector<std::string>& stages);
    void AddShaderProgram(const std::string& name, const ref<ShaderProgram>& shaderProgram);

    const ref<ShaderProgram>& GetShaderProgram(const std::string& name);
    const MapOf<ref<ShaderProgram>>& GetShaderPrograms() const;

    bool HasShaderProgram(const std::string& name) const;

    /**
     * Textures
     */

    void LoadTexture(const std::string& name, const std::string& path);
    void AddTexture(const std::string& name, const ref<Texture>& texture);

    const ref<Texture>& GetTexture(const std::string& name);
    const MapOf<ref<Texture>>& GetTextures() const;

    bool HasTexture(const std::string& name) const;

    /**
     * VAOs
     */

    void AddVAO(const std::string& name, const ref<VertexArray>& vao);

    const ref<VertexArray>& GetVAO(const std::string& name);
    const MapOf<ref<VertexArray>>& GetVAOs() const;

    bool HasVAO(const std::string& name) const;

private:
    ModelLibrary m_ModelLib;
    ShaderProgramLibrary m_ShaderProgramLib;
    TextureLibrary m_TextureLib;
    MaterialLibrary m_MaterialLib;
    MeshLibrary m_MeshLib;
    VertexArrayLibrary m_VAOLib;

private:
    static void InitDefaultModels();
    static void InitDefaultShaderPrograms();
    static void InitDefaultTextures();
    static void InitDefaultMaterials();
    static void InitDefaultVAOs();
    static void InitDefaultMeshes();

private:
        // should be Scope? 
        // doesn't matter, at termination it's insignificant if this thing is scoped or ref + lifetime of this is the whole app run
    inline static ref<ResourceManager> s_Instance = nullptr;
};

}
