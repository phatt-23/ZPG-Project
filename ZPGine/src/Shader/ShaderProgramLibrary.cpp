#include "ShaderProgramLibrary.h"
#include "Debug/Asserter.h"

namespace ZPG {

ShaderProgramLibrary::ShaderProgramLibrary() 
: m_ShaderProgramMap() {
    m_ShaderProgramMap["STOP"] = nullptr;
}

ShaderProgramLibrary::~ShaderProgramLibrary() {
}

void ShaderProgramLibrary::AddShaderProgram(const std::string& name, const Ref<ShaderProgram>& shaderProgram) {
    ZPG_CORE_ASSERT(!Exists(name), "The shader program name is already present in the shader libarary: {}");
    m_ShaderProgramMap[name] = shaderProgram;
}

Ref<ShaderProgram>& ShaderProgramLibrary::GetShaderProgram(const std::string& name) {
    ZPG_CORE_ASSERT(Exists(name), "Requested shader program doesn't exist: {}", name);
    return m_ShaderProgramMap[name];
}

bool ShaderProgramLibrary::Exists(const std::string& name) const { 
    return m_ShaderProgramMap.contains(name); 
}

}
