//
// Created by phatt on 11/14/25.
//

#ifndef WORKSPACE_TEXTURE2D_ARRAY_H
#define WORKSPACE_TEXTURE2D_ARRAY_H

#include "Texture.h"
#include "Buffer/FrameBufferAttachment.h"
#include "Texture/TextureDataFormat.h"

namespace ZPG
{
    class FrameBuffer;

    class Texture2DArray : public Texture
    {
    public:
        virtual ~Texture2DArray() override = default;

        TextureType GetTextureType() const override { return TextureType::Texture2DArray; }

        virtual u32 GetArraySize() const = 0;

        virtual void Resize(int width, int height, int slices) = 0;

        static ref<Texture2DArray> Create(int width, int height, int arraySize, TextureDataFormat dataFormat);
        static ref<Texture2DArray> Create(const std::string& name, int width, int height, int arraySize, TextureDataFormat dataFormat);
    };

} // ZPG

#endif //WORKSPACE_TEXTURE2D_ARRAY_H