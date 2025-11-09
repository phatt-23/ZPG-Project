#include "OpenGLIndexBuffer.h"
#include "OpenGLCore.h"
#include <glad/glad.h>

#include "Profiling/Instrumentor.h"

namespace ZPG {

OpenGLIndexBuffer::OpenGLIndexBuffer(const u32* indices, u32 count) : m_Count(count) {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glCreateBuffers(1, &m_RendererID));
    ZPG_OPENGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    ZPG_OPENGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW));
}
  
OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL( glDeleteBuffers(1, &m_RendererID) );
}
  
void OpenGLIndexBuffer::Bind() const {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
}
  
void OpenGLIndexBuffer::Unbind() const {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}
  
u32 OpenGLIndexBuffer::GetCount() const {
    ZPG_PROFILE_FUNCTION();
    return m_Count;
}

}
