#include "OpenGLFrameBuffer.h"

#include "OpenGLCore.h"
#include "OpenGLMapper.h"
#include "Renderer/RenderCommand.h"
#include "Texture/Texture.h"

namespace ZPG
{
    namespace Utils
    {
        std::string ToString(FrameBufferAttachmentType attachmentType)
        {
            switch (attachmentType) {
                case FrameBufferAttachmentType::Color: return "ColorAttachment";
                case FrameBufferAttachmentType::Depth: return "DepthAttachment";
                case FrameBufferAttachmentType::Stencil: return "StencilAttachment";
                case FrameBufferAttachmentType::DepthStencil: return "DepthStencilAttachment";
            }
            ZPG_UNREACHABLE();
        }
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
        : m_Specification(spec)
    {
        Invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        ZPG_OPENGL_CALL(glDeleteFramebuffers(1, &m_RendererID));
    }

    void OpenGLFrameBuffer::Bind() {
        ZPG_OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
        RenderCommand::SetViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFrameBuffer::Unbind() {
        ZPG_OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void OpenGLFrameBuffer::Invalidate() {
        if (m_RendererID != 0) {
            m_TextureAttachments.clear();
            m_ColorTextureAttachments.clear();
            ZPG_OPENGL_CALL(glDeleteFramebuffers(1, &m_RendererID));
        }

        ZPG_OPENGL_CALL(glGenFramebuffers(1, &m_RendererID));
        ZPG_OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

        for (const auto& frameBufferAttachment : m_Specification.Attachments)
        {
            std::string textureName = Utils::ToString(frameBufferAttachment.AttachmentType) + std::to_string(frameBufferAttachment.Index);

            ref<Texture> texture = Texture::Create(textureName, m_Specification.Width, m_Specification.Height, frameBufferAttachment.DataFormat);

            m_TextureAttachments[frameBufferAttachment] = texture;

            texture->AttachToFrameBuffer(m_RendererID, frameBufferAttachment);

            if (frameBufferAttachment.AttachmentType == FrameBufferAttachmentType::Color)
            {
                m_ColorTextureAttachments[frameBufferAttachment] = texture;
            }
        }

        std::vector<GLenum> drawBuffers;
        for (auto& colorAttachment : m_ColorTextureAttachments | std::views::keys) {
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + colorAttachment.Index);
        }
        std::ranges::sort(drawBuffers);

        ZPG_OPENGL_CALL(glDrawBuffers((GLsizei)drawBuffers.size(), drawBuffers.data()));

        int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        ZPG_CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer isn't complete - OpenGL status code: %d", status);

        ZPG_OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void OpenGLFrameBuffer::Resize(u32 width, u32 height) {
        ZPG_CORE_ASSERT(m_Specification.Resizable, "The framebuffer must be resizable.");

        if (width <= 0 || height <= 0 || (width == m_Specification.Width && height == m_Specification.Height)) {
            return;
        }

        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }

    void OpenGLFrameBuffer::BindColorTextureAttachments() {
        for (auto& [attachment, colorTex] : m_ColorTextureAttachments) {
            colorTex->BindToSlot(attachment.Index);
        }
    }

    u32 OpenGLFrameBuffer::GetRendererID() const {
        return m_RendererID;
    }

    void OpenGLFrameBuffer::CopyAttachment(const ref<FrameBuffer>& readFBO, FrameBufferAttachmentType attachmentType)
    {
        readFBO->WriteAttachment(m_RendererID, m_Specification.Width, m_Specification.Height, attachmentType);
    }

    void OpenGLFrameBuffer::WriteAttachment(u32 writeFramebufferRendererID, u32 width, u32 height, FrameBufferAttachmentType attachmentType)
    {
        OpenGLMapper::OpenGLAttachmentMapping gl = OpenGLMapper::ToGL(attachmentType);

        ZPG_OPENGL_CALL(glBlitNamedFramebuffer(
            m_RendererID, writeFramebufferRendererID,
            0, 0, m_Specification.Width, m_Specification.Height,
            0, 0, width, height,
            gl.BufferBit, GL_NEAREST));
    }

    const FrameBufferSpecification& OpenGLFrameBuffer::GetSpecification() const {
        return m_Specification;
    }

    const std::unordered_map<FrameBufferAttachment, ref<Texture>>& OpenGLFrameBuffer::GetTextureAttachments() const {
        return m_TextureAttachments;
    }

    const std::unordered_map<FrameBufferAttachment, ref<Texture>>& OpenGLFrameBuffer::GetColorTextureAttachments() const {
        return m_ColorTextureAttachments;
    }

    i32 OpenGLFrameBuffer::ReadPixelInt(u32 x, u32 y, FrameBufferAttachmentType attachmentType, u32 index) const {
        auto it = std::ranges::find_if(m_TextureAttachments, [&](const std::pair<FrameBufferAttachment, ref<Texture>>& pair) {
            return pair.first.AttachmentType == attachmentType && pair.first.Index == index;
        });

        ZPG_CORE_ASSERT(it != m_TextureAttachments.end(), "Attachment with the type {} and index {} isn't present in the FrameBuffer", (u32)attachmentType, index);

        OpenGLMapper::OpenGLAttachmentMapping glAttachment = OpenGLMapper::ToGL(it->first.AttachmentType);
        OpenGLMapper::OpenGLFormatMapping glDataFormat = OpenGLMapper::ToGL(it->first.DataFormat);

        ZPG_CORE_ASSERT(glDataFormat.Format == GL_RED_INTEGER);
        ZPG_CORE_ASSERT(glDataFormat.SampleType == GL_INT);

        i32 pixelData = 0;
        glReadBuffer(glAttachment.Attachment + index);
        glReadPixels(x, y, 1, 1, glDataFormat.Format, glDataFormat.SampleType, &pixelData);

        return pixelData;
    }

    v4 OpenGLFrameBuffer::ReadPixelFloat4(u32 x, u32 y, FrameBufferAttachmentType attachmentType, u32 index) const {
        auto it = std::ranges::find_if(m_TextureAttachments, [&](const std::pair<FrameBufferAttachment, ref<Texture>>& pair) {
            return pair.first.AttachmentType == attachmentType && pair.first.Index == index;
        });

        ZPG_CORE_ASSERT(it != m_TextureAttachments.end(), "Attachment with the type {} and index {} isn't present in the FrameBuffer", (u32)attachmentType, index);

        OpenGLMapper::OpenGLAttachmentMapping glAttachment = OpenGLMapper::ToGL(it->first.AttachmentType);
        OpenGLMapper::OpenGLFormatMapping glDataFormat = OpenGLMapper::ToGL(it->first.DataFormat);

        ZPG_CORE_ASSERT(glDataFormat.Format == GL_RGBA);
        ZPG_CORE_ASSERT(glDataFormat.SampleType == GL_FLOAT);

        v4 pixelData{0};
        glReadBuffer(glAttachment.Attachment + index);
        glReadPixels(x, y, 1, 1, glDataFormat.Format, glDataFormat.SampleType, &pixelData);

        return pixelData;
    }

    glm::u8vec4 OpenGLFrameBuffer::ReadPixelByte4(u32 x, u32 y, FrameBufferAttachmentType attachmentType, u32 index) const {
        auto it = std::ranges::find_if(m_TextureAttachments, [&](const std::pair<FrameBufferAttachment, ref<Texture>>& pair) {
            return pair.first.AttachmentType == attachmentType && pair.first.Index == index;
        });

        ZPG_CORE_ASSERT(it != m_TextureAttachments.end(), "Attachment with the type {} and index {} isn't present in the FrameBuffer", (u32)attachmentType, index);

        OpenGLMapper::OpenGLAttachmentMapping glAttachment = OpenGLMapper::ToGL(it->first.AttachmentType);
        OpenGLMapper::OpenGLFormatMapping glDataFormat = OpenGLMapper::ToGL(it->first.DataFormat);

        ZPG_CORE_ASSERT(glDataFormat.Format == GL_RGBA);
        ZPG_CORE_ASSERT(glDataFormat.SampleType == GL_UNSIGNED_BYTE);

        glm::u8vec4 pixelData{0};
        glReadBuffer(glAttachment.Attachment + index);
        glReadPixels(x, y, 1, 1, glDataFormat.Format, glDataFormat.SampleType, &pixelData);

        return pixelData;
    }

}
