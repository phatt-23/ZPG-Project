#include "Texture.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/Renderer.h"

namespace ZPG {

ref<Texture> Texture::Create(const std::string& filepath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: 
        return MakeRef<OpenGLTexture>(filepath);
    case RendererAPI::None:
    default:
        ZPG_UNREACHABLE();
    }
}
ref<Texture> Texture::Create(const std::string& name, const std::string& filepath) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: 
        return MakeRef<OpenGLTexture>(name, filepath);
    case RendererAPI::None:
    default:
        ZPG_UNREACHABLE();
    }
}
ref<Texture> Texture::Create(const std::string& name, u32 width, u32 height) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: 
        return MakeRef<OpenGLTexture>(name, width, height);
    case RendererAPI::None:
    default:
        ZPG_UNREACHABLE();
    }
}

}
