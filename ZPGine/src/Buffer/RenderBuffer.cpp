#include "RenderBuffer.h"
#include "Platform/OpenGL/OpenGLCore.h"

namespace ZPG {


RenderBuffer::RenderBuffer(u32 width, u32 height, BufferDataFormat dataFormat) 
: 
    m_Width(width)
, 
    m_Height(height) 
, 
    m_DataFormat(dataFormat)
{

    ZPG_OPENGL_CALL(glGenRenderbuffers(1, &m_RendererID));

    Bind();
  
    auto gl = BufferDataFormat::ToGL(m_DataFormat.Format);

    ZPG_OPENGL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, gl.InternalFormat, m_Width, m_Height));

    Unbind();
}

RenderBuffer::~RenderBuffer() {
    ZPG_OPENGL_CALL(glDeleteRenderbuffers(GL_RENDERBUFFER, &m_RendererID));
}

void RenderBuffer::Bind() {
    ZPG_OPENGL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}

void RenderBuffer::Unbind() {
    ZPG_OPENGL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void RenderBuffer::AttachToFrameBuffer(
    u32 frameBufferID, 
    Attachment attachment
) {
    auto gl = Attachment::ToGL(attachment.Type);
    
    Bind();

    // GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer
    ZPG_OPENGL_CALL(
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, 
            gl.Attachment + attachment.Index,
            GL_RENDERBUFFER, 
            m_RendererID));
}


ref<RenderBuffer> RenderBuffer::Create(u32 width, u32 height, BufferDataFormat dataFormat) {
    return MakeRef(new RenderBuffer(width, height, dataFormat));
}

}
