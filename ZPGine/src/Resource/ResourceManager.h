#pragma once

#include "Model/ModelLibrary.h"
#include "Shader/ShaderProgramLibrary.h"
#include "Texture/TextureLibrary.h"
#include "Material/MaterialLibrary.h"

namespace ZPG {



// Lifetime - whole duration of the application
//
// The resource manager holds all the shared assets that persist throughout the application run.
// It is implemented as a singleton, but it also allows the client to instantiate their own res manager
// and inject that into the scenes and layers, although by default, if nothing is passed (nullptr is default),
// then the global ResourceManager::Get() is 'injected' instead. The client doesn't care either way,
// the client code will look the same (m_ResManager.GetShaderProgram("Phong") could fetch the shader program from
// global resources or locally owned res manager).
// I don't think that the local variant will be of much use, but it is nice to have this option for testing and so on.
class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // called from application
    static void Init();
    static void Shutdown();

    static ResourceManager& GetGlobal() { return *s_Instance; }
    static ref<ResourceManager> GetGlobalRef() { return s_Instance; }

    // Model
    void LoadModel(const std::string& name, const std::string& path);
    void AddModel(const std::string& name, const ref<Model>& model);
    const ref<Model>& GetModel(const std::string& name) const;
    bool HasModel(const std::string& name) const;

    // ShaderProgram
    void LoadShaderProgram(const std::string& name, 
        const std::string& vertexShaderPath, 
        const std::string& fragmentShaderPath);
    void AddShaderProgram(const std::string& name, const ref<ShaderProgram>& shaderProgram);
    const ref<ShaderProgram>& GetShaderProgram(const std::string& name);
    bool HasShaderProgram(const std::string& name) const;

    // Texture
    void LoadTexture(const std::string& name, const std::string& path);
    void AddTexture(const std::string& name, const ref<Texture>& texture);
    const ref<Texture>& GetTexture(const std::string& name);
    bool HasTexture(const std::string& name) const;

    // Material
    void AddMaterial(const std::string& name, const ref<Material>& material);
    ref<Material> GetMaterial(const std::string& name);
    bool HasMaterial(const std::string& name) const;

private:
    ModelLibrary m_ModelLib;
    ShaderProgramLibrary m_ShaderProgramLib;
    TextureLibrary m_TextureLib;
    MaterialLibrary m_MaterialLib;

private:
    static void InitDefaultModels();
    static void InitDefaultShaderPrograms();
    static void InitDefaultTextures();
    static void InitDefaultMaterials();

private:
        // should be Scope? 
        // doesn't matter, at termination it's insignificant if this thing is scoped or ref + lifetime of this is the whole app run
    inline static ref<ResourceManager> s_Instance = nullptr;
};

}
