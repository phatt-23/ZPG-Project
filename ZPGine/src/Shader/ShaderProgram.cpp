#include "ShaderProgram.h"
#include "Debug/Asserter.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShaderProgram.h"

namespace ZPG {

Ref<ShaderProgram> ShaderProgram::Create(const std::string& filePath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLShaderProgram>(filePath);
    case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}
Ref<ShaderProgram> ShaderProgram::Create(const std::string& name, const std::string& filePath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLShaderProgram>(name, filePath);
    case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}
Ref<ShaderProgram> ShaderProgram::Create(const std::string& name, const std::vector<Ref<Shader>>& shaders) {
    u32 shaderBits = 0b0;

    for (auto shader : shaders) {
        Shader::ShaderType shaderType = shader->GetShaderType();
        
        ZPG_CORE_ASSERT((u32)shaderType != Shader::None, "Shader type must not be None!");
        ZPG_CORE_ASSERT((shaderBits & (u32)shaderType) == 0, 
            "There should be only one shader of each type. Instead got multiple shaders of type: {}", 
            Shader::ShaderTypeToString(shaderType));
            
        shaderBits |= (u32)shaderType;
    }

    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLShaderProgram>(name, shaders);
    case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}

}
