//
// Created by phatt on 10/18/25.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "FrameBufferAttachment.h"

namespace ZPG {

    class Texture;

    struct FrameBufferSpecification {
        u32 Width;
        u32 Height;

        std::vector<FrameBufferAttachment> Attachments;
    };

    class FrameBuffer {
    public:
        virtual ~FrameBuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual u32 GetRendererID() const = 0;

        virtual void Invalidate() = 0;
        virtual void Resize(u32 width, u32 height) = 0;
        virtual void BindColorTextureAttachments() = 0;

        virtual const FrameBufferSpecification& GetSpecification() const = 0;
        virtual const std::unordered_map<FrameBufferAttachment, ref<Texture>>& GetTextureAttachments() const = 0;
        virtual const std::unordered_map<FrameBufferAttachment, ref<Texture>>& GetColorTextureAttachments() const = 0;

        virtual void CopyAttachment(const ref<FrameBuffer>& srcFramebuffer, FrameBufferAttachmentType attachmentType) = 0;
        virtual void WriteAttachment(u32 destFramebufferRendererID, u32 width, u32 height, FrameBufferAttachmentType attachmentType) = 0;

        virtual i32 ReadPixelInt(u32 x, u32 y, FrameBufferAttachmentType attachmentType = FrameBufferAttachmentType::Color, u32 index = 0) const = 0;
        virtual v4 ReadPixelFloat4(u32 x, u32 y, FrameBufferAttachmentType attachmentType = FrameBufferAttachmentType::Color, u32 index = 0) const = 0;
        virtual glm::u8vec4 ReadPixelByte4(u32 x, u32 y, FrameBufferAttachmentType attachmentType = FrameBufferAttachmentType::Color, u32 index = 0) const = 0;

        static ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
    };

}


#endif //FRAMEBUFFER_H
