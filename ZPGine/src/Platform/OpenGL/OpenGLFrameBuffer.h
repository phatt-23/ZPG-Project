#pragma once

#include "Buffer/FrameBuffer.h"

namespace ZPG {

	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);

		~OpenGLFrameBuffer() override;

		void Bind() override;
		void Unbind() override;

		void Invalidate() override;
		void Resize(u32 width, u32 height) override;
		void BindColorTextureAttachments() override;

		u32 GetRendererID() const override;

		void CopyAttachment(const ref<FrameBuffer>& readFBO, AttachmentType attachmentType) override;
		void WriteAttachment(u32 writeFramebufferRendererID, u32 width, u32 height, AttachmentType attachmentType) override;

		const FrameBufferSpecification& GetSpecification() const override;
		const std::unordered_map<RenderAttachment, ref<Texture>>& GetTextureAttachments() const override;
		const std::unordered_map<RenderAttachment, ref<Texture>>& GetColorTextureAttachments() const override;

	private:
		u32 m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::unordered_map<RenderAttachment, ref<Texture>> m_TextureAttachments;
		std::unordered_map<RenderAttachment, ref<Texture>> m_ColorTextureAttachments;
	};

}