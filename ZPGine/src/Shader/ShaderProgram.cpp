#include "ShaderProgram.h"
#include "Debug/Asserter.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShaderProgram.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

ref<ShaderProgram> ShaderProgram::Create(const std::string& filePath)
{
    ZPG_PROFILE_FUNCTION();
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL: return MakeRef<OpenGLShaderProgram>(filePath);
        case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}

ref<ShaderProgram> ShaderProgram::Create(const std::string& name, const std::string& filePath)
{
    ZPG_PROFILE_FUNCTION();
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL: return MakeRef<OpenGLShaderProgram>(name, filePath);
        case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}

ref<ShaderProgram> ShaderProgram::Create(const std::string& name, const std::vector<ref<Shader>>& shaders)
{
    ZPG_PROFILE_FUNCTION();
    u32 shaderBits = 0b0;

    for (auto shader : shaders)
    {
        Shader::ShaderType shaderType = shader->GetShaderType();
        
        ZPG_CORE_ASSERT((u32)shaderType != Shader::None, "Shader type must not be None!");
        ZPG_CORE_ASSERT((shaderBits & (u32)shaderType) == 0, 
            "There should be only one shader of each type. Instead got multiple shaders of type: {}", 
            Shader::ShaderTypeToString(shaderType));
            
        shaderBits |= (u32)shaderType;
    }

    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL: return MakeRef<OpenGLShaderProgram>(name, shaders);
        case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}

}
