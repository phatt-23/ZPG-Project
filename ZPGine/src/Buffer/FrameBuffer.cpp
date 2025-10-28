//
// Created by phatt on 10/18/25.
//

#include "FrameBuffer.h"
#include "Platform/OpenGL/OpenGLCore.h"
#include "RenderBuffer.h"
#include "Texture/Texture.h"

namespace ZPG {

FrameBuffer::FrameBuffer(
    std::unordered_multimap<AttachmentType::Type, ref<Texture>> textureAttachments,
    std::unordered_multimap<AttachmentType::Type, ref<RenderBuffer>> rboAttachments
) {
    ZPG_OPENGL_CALL(glGenFramebuffers(1, &m_RendererID));
    Bind();

    for (auto& [attachType, texture] : textureAttachments) {
        m_TextureAttachments.insert({attachType, texture});

        if (attachType == AttachmentType::Color) {
            texture->AttachToFrameBuffer(
                m_RendererID, 
                attachType, 
                m_ColorTextureAttachments.size());
        
            m_ColorTextureAttachments.push_back(texture);
        }
        else {
            texture->AttachToFrameBuffer(m_RendererID, attachType);
        }

    }

    std::vector<GLenum> drawBuffers;
    drawBuffers.reserve(m_ColorTextureAttachments.size());

    for (int i = 0; i < m_ColorTextureAttachments.size(); i++) {
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(drawBuffers.size(), drawBuffers.data());

    for (auto& [attachType, rbo] : rboAttachments) {
        m_RBOAttachments.insert({attachType, rbo});

        rbo->AttachToFrameBuffer(m_RendererID, attachType);
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

}
