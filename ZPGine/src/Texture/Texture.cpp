#include "Texture.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/Renderer.h"

namespace ZPG {

ref<Texture> Texture::Create(
    const std::string& filepath,
    BufferDataFormat format
) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return MakeRef(new OpenGLTexture(filepath, format));
    case RendererAPI::None:
    default: ZPG_UNREACHABLE();
    }
}

ref<Texture> Texture::Create(
    const std::string& name, 
    const std::string& filepath,
    BufferDataFormat format
) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return MakeRef(new OpenGLTexture(name, filepath, format));
    case RendererAPI::None:
    default: ZPG_UNREACHABLE();
    }
}

ref<Texture> Texture::Create(
    const std::string& name, 
    u32 width, 
    u32 height,
    BufferDataFormat format
) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return MakeRef(new OpenGLTexture(name, width, height, format));
    case RendererAPI::None:
    default: ZPG_UNREACHABLE();
    }
}

}
