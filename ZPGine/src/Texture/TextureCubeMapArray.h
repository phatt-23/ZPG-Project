//
// Created by phatt on 11/14/25.
//

#ifndef WORKSPACE_TEXTURE_CUBE_MAP_ARRAY_H
#define WORKSPACE_TEXTURE_CUBE_MAP_ARRAY_H

#include "Texture.h"
#include "TextureDataFormat.h"
#include "TextureType.h"

namespace ZPG
{
    struct FrameBufferAttachment;

    class TextureCubeMapArray : public Texture
    {
    public:
        virtual ~TextureCubeMapArray() override = default;

        TextureType GetTextureType() const override { return TextureType::TextureCubeMapArray; }

        virtual u32 GetLength() const = 0;
        virtual u32 GetArraySize() const = 0;

        virtual void Resize(int length, int slices) = 0;

        static ref<TextureCubeMapArray> Create(int length, int arraySize, TextureDataFormat dataFormat);
        static ref<TextureCubeMapArray> Create(const std::string& name, int length, int arraySize, TextureDataFormat dataFormat);
    };
} // ZPG

#endif