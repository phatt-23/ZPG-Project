//
// Created by phatt on 11/14/25.
//

#include "Texture2DArray.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLTexture2DArray.h"
#include "Renderer/RendererAPI.h"

namespace ZPG
{
    ref<Texture2DArray> Texture2DArray::Create(int width, int height, int arraySize, TextureDataFormat dataFormat)
    {
        return Texture2DArray::Create("Texture2DArray", width, height, arraySize, dataFormat);
    }

    ref<Texture2DArray> Texture2DArray::Create(const std::string& name, int width, int height, int arraySize, TextureDataFormat dataFormat)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::None: ZPG_UNREACHABLE("RendererAPI::None not supported"); break;
            case RendererAPI::OpenGL: return MakeRef(new OpenGLTexture2DArray(name, width, height, arraySize, dataFormat));
        }

        return nullptr;
    }

} // ZPG