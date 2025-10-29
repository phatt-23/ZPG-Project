//
// Created by phatt on 10/18/25.
//

#include "FrameBuffer.h"
#include "Platform/OpenGL/OpenGLCore.h"
#include "RenderBuffer.h"
#include "Texture/Texture.h"

namespace ZPG {

FrameBuffer::FrameBuffer(
    const std::unordered_map<Attachment, ref<Texture>>& textureAttachments,
    const std::unordered_map<Attachment, ref<RenderBuffer>>& rboAttachments
) {
    ZPG_OPENGL_CALL(glGenFramebuffers(1, &m_RendererID));
    Bind();

    for (auto& [attachment, texture] : textureAttachments) {
        m_TextureAttachments[attachment] = texture;

        if (attachment.Type == Attachment::Color) {
            m_ColorTextureAttachments[attachment] = texture;
        }

        texture->AttachToFrameBuffer(m_RendererID, attachment);
    }

    std::vector<GLenum> drawBuffers;
    drawBuffers.reserve(m_ColorTextureAttachments.size());

    for (int i = 0; i < m_ColorTextureAttachments.size(); i++) {
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(drawBuffers.size(), drawBuffers.data());

    for (auto& [attachment, rbo] : rboAttachments) {
        m_RBOAttachments[attachment] = rbo;

        rbo->AttachToFrameBuffer(m_RendererID, attachment);
    }

    int status;
    ZPG_OPENGL_CALL(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        ZPG_UNREACHABLE("Framebuffer isn't complete - OpenGL status code: %d", status); 
    }

    Unbind();
}

FrameBuffer::~FrameBuffer() {
    ZPG_OPENGL_CALL(glDeleteFramebuffers(1, &m_RendererID));
}

void FrameBuffer::Bind() {
    ZPG_OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::Unbind() {
    ZPG_OPENGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

const FrameBuffer::AttachMap<std::shared_ptr<Texture>>& FrameBuffer::GetTextureAttachments() const {
    return m_TextureAttachments;
}

const FrameBuffer::AttachMap<std::shared_ptr<Texture>>& FrameBuffer::GetColorTextureAttachments() const {
    return m_ColorTextureAttachments;
}

const FrameBuffer::AttachMap<std::shared_ptr<RenderBuffer>>& FrameBuffer::GetRenderBufferAttachments() const {
    return m_RBOAttachments;
}

void FrameBuffer::BindColorTextureAttachments() {
    for (auto& [attachment, colorTex] : m_ColorTextureAttachments) {
        colorTex->BindToSlot(attachment.Index);
    }
}
}
