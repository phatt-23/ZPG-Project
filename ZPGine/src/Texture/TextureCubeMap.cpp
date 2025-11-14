//
// Created by phatt on 11/3/25.
//

#include "TextureCubeMap.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLTextureCubeMap.h"
#include "Renderer/RendererAPI.h"

namespace ZPG
{
    ref<TextureCubeMap> TextureCubeMap::Create(const std::string& name, const std::array<std::string, 6>& faceFilepaths, TextureDataFormat dataFormat)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::None: return nullptr;
            case RendererAPI::OpenGL: return MakeRef(new OpenGLTextureCubeMap(name, faceFilepaths, dataFormat));
        }

        ZPG_UNREACHABLE();
        return nullptr;
    }

    ref<TextureCubeMap> TextureCubeMap::Create(const std::string& name, u32 length, TextureDataFormat dataFormat)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::None: return nullptr;
            case RendererAPI::OpenGL: return MakeRef(new OpenGLTextureCubeMap(name, length, dataFormat));
        }

        ZPG_UNREACHABLE();
        return nullptr;
    }
} // ZPG
