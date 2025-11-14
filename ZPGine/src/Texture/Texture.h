//
// Created by phatt on 11/14/25.
//

#ifndef WORKSPACE_TEXTURE_H
#define WORKSPACE_TEXTURE_H

#include "TextureType.h"
#include "TextureDataFormat.h"

namespace ZPG
{
    struct FrameBufferAttachment;

    class Texture
    {
    public:
        virtual ~Texture() = default;
        virtual TextureType GetTextureType() const { return TextureType::None; }

        virtual void BindToSlot(u32 slotIndex) = 0;

        virtual void AttachToFrameBuffer(u32 frameBufferRendererID, const FrameBufferAttachment& frameBufferAttachment) = 0;

        virtual void Invalidate() = 0;

        virtual u32 GetWidth() const = 0;
        virtual u32 GetHeight() const = 0;
        virtual TextureDataFormat GetDataFormat() const = 0;
        virtual const std::string& GetName() const = 0;

        // for debugging
        virtual u32 GetRendererID() const = 0;
    };

}

#endif //WORKSPACE_TEXTURE_H