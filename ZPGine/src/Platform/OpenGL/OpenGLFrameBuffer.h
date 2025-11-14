#pragma once

#include "Buffer/FrameBuffer.h"

namespace ZPG {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);

		~OpenGLFrameBuffer() override;

		void Bind() override;
		void Unbind() override;

		void Invalidate() override;
		void Resize(u32 width, u32 height) override;
		void BindColorTextureAttachments() override;

		u32 GetRendererID() const override;

		void CopyAttachment(const ref<FrameBuffer>& readFBO, FrameBufferAttachmentType attachmentType) override;
		void WriteAttachment(u32 writeFramebufferRendererID, u32 width, u32 height, FrameBufferAttachmentType attachmentType) override;

        void AttachTexture(const ref<Texture>& texture, const FrameBufferAttachment& frameBufferAttachment) override;

		const FrameBufferSpecification& GetSpecification() const override;
		const umap<FrameBufferAttachment, ref<Texture>>& GetTextureAttachments() const override;
		const umap<FrameBufferAttachment, ref<Texture>>& GetColorTextureAttachments() const override;

        i32 ReadPixelInt(u32 x, u32 y, FrameBufferAttachmentType attachmentType = FrameBufferAttachmentType::Color, u32 index = 0) const override;
		v4 ReadPixelFloat4(u32 x, u32 y, FrameBufferAttachmentType attachmentType = FrameBufferAttachmentType::Color, u32 index = 0) const override;
		glm::u8vec4 ReadPixelByte4(u32 x, u32 y, FrameBufferAttachmentType attachmentType = FrameBufferAttachmentType::Color, u32 index = 0) const override;

	private:
		u32 m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		umap<FrameBufferAttachment, ref<Texture>> m_TextureAttachments;
		umap<FrameBufferAttachment, ref<Texture>> m_ColorTextureAttachments;
	};

}