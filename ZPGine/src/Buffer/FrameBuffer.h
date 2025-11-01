//
// Created by phatt on 10/18/25.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "RenderAttachment.h"

namespace ZPG {

    class Texture;

    struct FrameBufferSpecification {
        u32 Width;
        u32 Height;

        std::unordered_map<std::string, RenderAttachment> Attachments;
    };

    class FrameBuffer {
    public:
        virtual ~FrameBuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Invalidate() = 0;
        virtual void Resize(u32 width, u32 height) = 0;
        virtual void BindColorTextureAttachments() = 0;

        virtual const FrameBufferSpecification& GetSpecification() const = 0;
        virtual const std::unordered_map<RenderAttachment, ref<Texture>>& GetTextureAttachments() const = 0;
        virtual const std::unordered_map<RenderAttachment, ref<Texture>>& GetColorTextureAttachments() const = 0;

        virtual u32 GetRendererID() const = 0;

        virtual void CopyAttachment(const ref<FrameBuffer>& srcFramebuffer, AttachmentType attachmentType) = 0;
        virtual void WriteAttachment(u32 destFramebufferRendererID, u32 width, u32 height, AttachmentType attachmentType) = 0;

        static ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
    };

}


#endif //FRAMEBUFFER_H
