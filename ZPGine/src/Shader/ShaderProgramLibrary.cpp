#include "ShaderProgramLibrary.h"
#include "Debug/Asserter.h"

namespace ZPG {

ShaderProgramLibrary::ShaderProgramLibrary() {
}
ShaderProgramLibrary::~ShaderProgramLibrary() {
}
void ShaderProgramLibrary::AddShaderProgram(const std::string& name, const Ref<ShaderProgram>& shaderProgram) {
    ZPG_CORE_ASSERT(!Exists(name), "The shader program name is already present in the shader libarary: {}", name);
    m_ShaderPrograms[name] = shaderProgram;
}
const Ref<ShaderProgram>& ShaderProgramLibrary::GetShaderProgram(const std::string& name) {
    ZPG_CORE_ASSERT(Exists(name), "Requested shader program doesn't exist: {}", name);
    return m_ShaderPrograms[name];
}
bool ShaderProgramLibrary::Exists(const std::string& name) const { 
    return m_ShaderPrograms.contains(name); 
}

}
