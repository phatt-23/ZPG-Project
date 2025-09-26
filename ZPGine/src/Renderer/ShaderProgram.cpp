#include "ShaderProgram.h"
#include "Debug/Asserter.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShaderProgram.h"

namespace ZPG {

Ref<ShaderProgram> ShaderProgram::Create(const std::string& path) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLShaderProgram>(path);
    case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}
Ref<ShaderProgram> ShaderProgram::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLShaderProgram>(name, vertexSource, fragmentSource);
    case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}

}
