#pragma once

#include "BufferDataFormat.h"
#include "AttachmentType.h"

namespace ZPG {

class RenderBuffer {
public:

    RenderBuffer(u32 width, u32 height, BufferDataFormat dataFormat);
    ~RenderBuffer();

    void Bind(); 
    void Unbind();

    void AttachToFrameBuffer(u32 frameBufferID, AttachmentType::Type attachmentType);

    static ref<RenderBuffer> Create(u32 width, u32 height, BufferDataFormat dataFormat);

private:
    u32 m_RendererID;
    u32 m_Width;
    u32 m_Height;
    BufferDataFormat m_DataFormat;
};

}
