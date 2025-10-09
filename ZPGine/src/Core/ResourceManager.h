#pragma once

#include "Model/ModelLibrary.h"
#include "Shader/ShaderProgramLibrary.h"

namespace ZPG {


// Lifetime - whole duration of the application
//
// The resource manager holds all the shared assets that persist throughout the application run.
// It is implemented as a singleton, but it also allows the client to instantiate their own res manager
// and inject that into the scenes and layers, although by default, if nothing is passed (nullptr is default),
// then the global ResourceManager::Get() is 'injected' instead. The client doesn't care either way,
// the client code will look the same (m_ResManager.GetShaderProgram("Phong") could fetch the shader program from
// global resources or localy owned res manager).
// I don't think that the local variant will be of much use, but it is nice to have this option for testing and so on.
class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // called from application
    static void Init();
    static void Shutdown();

    static ResourceManager& GetGlobal() { return *s_Instance; }

    void LoadModel(const std::string& name, const std::string& path);
    const Ref<Model>& GetModel(const std::string& name) const;

    void LoadShaderProgram(const std::string& name, 
        const std::string& vertexShaderPath, 
        const std::string& fragmentShaderPath);

    const Ref<ShaderProgram>& GetShaderProgram(const std::string& name);

private:
    ModelLibrary m_ModelLib;
    ShaderProgramLibrary m_ShaderProgramLib;
private:
    inline static ResourceManager* s_Instance = nullptr;
};

}