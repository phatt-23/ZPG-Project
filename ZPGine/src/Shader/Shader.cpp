#include "Shader.h"

#include "Debug/Asserter.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace ZPG {

std::string Shader::ShaderTypeToString(ShaderType type) {
    switch (type) {
    case Vertex: return "vertex";
    case Fragment: return "fragment";
    case Geometry: return "geometry";
    default:
        ZPG_UNREACHABLE("Unknown ShaderType.");
    }
}
Shader::ShaderType Shader::ShaderTypeFromString(const std::string& type) {
    if (type == "vertex") 
        return Shader::ShaderType::Vertex;
    if (type == "geometry")
        return Shader::ShaderType::Geometry;
    if (type == "fragment" || type == "pixel")
        return Shader::ShaderType::Fragment;
    ZPG_UNREACHABLE("Cannot convert string to ShaderType from provided string: {}", type);
}

ref<Shader> Shader::Create(const std::string& filepath) {
    switch (RendererAPI::GetAPI()) {
    case RendererAPI::OpenGL: return MakeRef<OpenGLShader>(filepath);
    default: ZPG_UNREACHABLE();
    }
}
ref<Shader> Shader::Create(const std::string& name, const std::string& filepath) {
    switch (RendererAPI::GetAPI()) {
    case RendererAPI::OpenGL: return MakeRef<OpenGLShader>(name, filepath);
    default: ZPG_UNREACHABLE();
    }
}
ref<Shader> Shader::CreateFromCode(const std::string& name, ShaderType type, const std::string& sourceCode) {
    switch (RendererAPI::GetAPI()) {
    case RendererAPI::OpenGL: return MakeRef<OpenGLShader>(name, type, sourceCode);
    default: ZPG_UNREACHABLE();
    }
}

}
