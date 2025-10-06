#include "Shader.h"
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
    else if (type == "geometry") 
        return Shader::ShaderType::Geometry;
    else if (type == "fragment" || type == "pixel") 
        return Shader::ShaderType::Fragment;
    ZPG_UNREACHABLE("Cannot convert string to ShaderType from provided string: {}", type);
}

Ref<Shader> Shader::Create(const std::string& filepath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLShader>(filepath);
    default: ZPG_UNREACHABLE();
    }
}
Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLShader>(name, filepath);
    default: ZPG_UNREACHABLE();
    }
}
Ref<Shader> Shader::CreateFromCode(const std::string& name, ShaderType type, const std::string& sourceCode) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLShader>(name, type, sourceCode);
    default: ZPG_UNREACHABLE();
    }
}

}
