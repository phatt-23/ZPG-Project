#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer.h"

namespace ZPG {

Ref<Texture> Texture::Create(const std::string& filepath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: 
        return CreateRef<OpenGLTexture>(filepath);
    case RendererAPI::None:
    default:
        ZPG_UNREACHABLE();
    }
}
Ref<Texture> Texture::Create(const std::string& name, const std::string& filepath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: 
        return CreateRef<OpenGLTexture>(name, filepath);
    case RendererAPI::None:
    default:
        ZPG_UNREACHABLE();
    }
}

}
