#include "OpenGLFrameBuffer.h"

#include "OpenGLCore.h"
#include "OpenGLMapper.h"
#include "Renderer/RenderCommand.h"
#include "Texture/Texture.h"

namespace ZPG {

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

    for (const auto& [attachmentName, renderAttachment] : m_Specification.Attachments) {
        ref<Texture> texture = Texture::Create(attachmentName, m_Specification.Width, m_Specification.Height, renderAttachment.BufferSpec.Format);

        m_TextureAttachments[renderAttachment] = texture;

        if (renderAttachment.BufferSpec.Type == AttachmentType::Color) {
            m_ColorTextureAttachments[renderAttachment] = texture;
        }

        texture->AttachToFrameBuffer(m_RendererID, renderAttachment);
    }

    std::vector<GLenum> drawBuffers;
    for (int i = 0; i < m_ColorTextureAttachments.size(); i++) {
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    ZPG_OPENGL_CALL(glDrawBuffers((GLsizei)drawBuffers.size(), drawBuffers.data()));

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    ZPG_CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer isn't complete - OpenGL status code: %d", status);

    ZPG_OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGLFrameBuffer::Resize(u32 width, u32 height) {
    if (width <= 0 || height <= 0 || (width == m_Specification.Width && height == m_Specification.Height)) {
        return;
    }

    m_Specification.Width = width;
    m_Specification.Height = height;
    Invalidate();
}

void OpenGLFrameBuffer::BindColorTextureAttachments() {
    for (auto& [attachment, colorTex] : m_ColorTextureAttachments) {
        colorTex->BindToSlot(attachment.BindingPoint);
    }
}

u32 OpenGLFrameBuffer::GetRendererID() const {
    return m_RendererID;
}

void OpenGLFrameBuffer::CopyAttachment(const ref<FrameBuffer>& readFBO, AttachmentType attachmentType)
{
    readFBO->WriteAttachment(m_RendererID, m_Specification.Width, m_Specification.Height, attachmentType);
}

void OpenGLFrameBuffer::WriteAttachment(u32 writeFramebufferRendererID, u32 width, u32 height, AttachmentType attachmentType)
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

const std::unordered_map<RenderAttachment, ref<Texture>>& OpenGLFrameBuffer::GetTextureAttachments() const {
    return m_TextureAttachments;
}

const std::unordered_map<RenderAttachment, ref<Texture>>& OpenGLFrameBuffer::GetColorTextureAttachments() const {
    return m_ColorTextureAttachments;
}

}
