#include "ResourceManager.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

ResourceManager::ResourceManager() 
: m_ModelLib(), m_ShaderProgramLib() {

}
ResourceManager::~ResourceManager() {

}

void ResourceManager::Init() {
    ZPG_CORE_ASSERT(s_Instance == nullptr, "The global resource manager was already instantiated.");
    s_Instance = new ResourceManager();
}

void ResourceManager::Shutdown() {
    ZPG_CORE_ASSERT(s_Instance != nullptr, "The resource manager that wasn't initialized can't be shutdown.");
    delete s_Instance;
    s_Instance = nullptr;
}

void ResourceManager::LoadModel(const std::string& name, const std::string& path) {
    m_ModelLib.LoadModel(name, path);
}
const Ref<Model>& ResourceManager::GetModel(const std::string& name) const {
    return m_ModelLib.GetModel(name);
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
    
const Ref<ShaderProgram>& ResourceManager::GetShaderProgram(const std::string& name) {
    return m_ShaderProgramLib.GetShaderProgram(name);
}

}