#include "ShaderProgramLibrary.h"
#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

ShaderProgramLibrary::ShaderProgramLibrary() 
: m_ShaderProgramMap() {
    ZPG_PROFILE_FUNCTION();
    m_ShaderProgramMap["STOP"] = nullptr;
}

ShaderProgramLibrary::~ShaderProgramLibrary() {
    ZPG_PROFILE_FUNCTION();
}

void ShaderProgramLibrary::AddShaderProgram(const std::string& name, const ref<ShaderProgram>& shaderProgram) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(!Exists(name), "The shader program name is already present in the shader library: {}", name);
    m_ShaderProgramMap[name] = shaderProgram;
}

ref<ShaderProgram>& ShaderProgramLibrary::GetShaderProgram(const std::string& name) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(Exists(name), "Requested shader program doesn't exist: {}", name);
    return m_ShaderProgramMap[name];
}

bool ShaderProgramLibrary::Exists(const std::string& name) const { 
    ZPG_PROFILE_FUNCTION();
    return m_ShaderProgramMap.contains(name);
}

const std::unordered_map<std::string, std::shared_ptr<ShaderProgram>>& ShaderProgramLibrary::GetShaders() const {
    ZPG_PROFILE_FUNCTION();
    return m_ShaderProgramMap;
}

}
