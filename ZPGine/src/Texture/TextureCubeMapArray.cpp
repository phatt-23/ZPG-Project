//
// Created by phatt on 11/14/25.
//

#include "TextureCubeMapArray.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLTextureCubeMapArray.h"
#include "Renderer/RendererAPI.h"

namespace ZPG
{
    ref<TextureCubeMapArray> TextureCubeMapArray::Create(int length, int arraySize, TextureDataFormat dataFormat)
    {
        return TextureCubeMapArray::Create("TextureCubeMapArray", length, arraySize, dataFormat);
    }

    ref<TextureCubeMapArray> TextureCubeMapArray::Create(const std::string& name, int length, int arraySize, TextureDataFormat dataFormat)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::None: ZPG_UNREACHABLE("RendererAPI::None not supported"); break;
            case RendererAPI::OpenGL: return MakeRef(new OpenGLTextureCubeMapArray(name, length, arraySize, dataFormat));
        }

        ZPG_UNREACHABLE();
        return nullptr;
    }
} // ZPG