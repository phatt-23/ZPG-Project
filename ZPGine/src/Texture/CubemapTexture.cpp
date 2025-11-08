//
// Created by phatt on 11/3/25.
//

#include "CubemapTexture.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLCubemapTexture.h"
#include "Renderer/Renderer.h"

namespace ZPG
{

    ref<CubemapTexture> CubemapTexture::Create(const std::string& name, const std::array<std::string, 6>& faceFilepaths, TextureDataFormat dataFormat)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::None: return nullptr;
            case RendererAPI::OpenGL: return MakeRef(new OpenGLCubemapTexture(name, faceFilepaths, dataFormat));
        }

        ZPG_UNREACHABLE();
    }

    ref<CubemapTexture> CubemapTexture::Create(const std::string& name, u32 width, u32 height, TextureDataFormat dataFormat)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::None: return nullptr;
            case RendererAPI::OpenGL: return MakeRef(new OpenGLCubemapTexture(name, width, height, dataFormat));
        }

        ZPG_UNREACHABLE();
    }
} // ZPG
