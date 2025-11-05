//
// Created by phatt on 9/22/25.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Buffer/FrameBufferAttachment.h"
#include "Buffer/FrameBufferAttachment.h"
#include "Buffer/FrameBuffer.h"

namespace ZPG {

class Texture {
public:
    virtual ~Texture() {}

    virtual const std::string& GetName() const = 0;
    virtual void BindToSlot(u32 slotIndex) = 0;
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0;

    virtual void SetData(const void *data, u32 size) = 0;

    virtual void AttachToFrameBuffer(u32 frameBufferID, FrameBufferAttachment frameBufferAttachment) = 0;

    virtual void Resize(u32 width, u32 height) = 0;

    virtual void Invalidate() = 0;

    virtual u32 GetRendererID() const = 0;

    static ref<Texture> Create(
        const std::string& filepath, 
        TextureDataFormat dataFormat = TextureDataFormat::RGBA8);

    static ref<Texture> Create(
        const std::string& name, 
        const std::string& filepath, 
        TextureDataFormat dataFormat = TextureDataFormat::RGBA8);                                                           /* There is no need for this dataFormat, it should be deduced from the loaded texture */

    static ref<Texture> Create(
        const std::string& name, 
        u32 width, 
        u32 height,
        TextureDataFormat dataFormat = TextureDataFormat::RGBA8);
};

}

#endif //TEXTURE_H
