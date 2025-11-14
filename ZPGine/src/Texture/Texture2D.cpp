#include "Texture2D.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Renderer/RendererAPI.h"

namespace ZPG {

ref<Texture2D> Texture2D::Create(const std::string& filepath, TextureDataFormat dataFormat)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL: return MakeRef(new OpenGLTexture2D(filepath, dataFormat));
        case RendererAPI::None:
        default: ZPG_UNREACHABLE();
    }
    return nullptr;
}

ref<Texture2D> Texture2D::Create(const std::string& name, const std::string& filepath, TextureDataFormat dataFormat)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL: return MakeRef(new OpenGLTexture2D(name, filepath, dataFormat));
        case RendererAPI::None:
        default: ZPG_UNREACHABLE();
    }
    return nullptr;
}

ref<Texture2D> Texture2D::Create(const std::string& name, u32 width, u32 height, TextureDataFormat dataFormat)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL: return MakeRef(new OpenGLTexture2D(name, width, height, dataFormat));
        case RendererAPI::None:
        default: ZPG_UNREACHABLE();
    }
    return nullptr;
}

}
