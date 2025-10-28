#include "OpenGLTexture.h"
#include "Debug/Asserter.h"
#include "Core/Core.h"
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>
#include "OpenGLCore.h"
#include "Buffer/BufferDataFormat.h"

namespace ZPG {

OpenGLTexture::OpenGLTexture(const std::string& filepath, BufferDataFormat dataFormat) 
: m_DataFormat(dataFormat) {
    m_Name = filepath.substr(
        filepath.find_last_of("/\\") + 1, 
        filepath.find_last_of('.') - filepath.find_last_of("/\\") - 1);

    LoadTexture(filepath);
}

OpenGLTexture::OpenGLTexture(const std::string& name, const std::string& filepath, BufferDataFormat dataFormat)
: m_Name(name)
, m_DataFormat(dataFormat) {
    LoadTexture(filepath);
}

OpenGLTexture::OpenGLTexture(const std::string& name, u32 width, u32 height, BufferDataFormat format) 
: m_Name(name) 
, m_Width(width)
, m_Height(height) 
, m_SampleSize(BufferDataFormat::ToGL(format.Format).SampleTypeSize)
, m_DataFormat(format) {
    CreateEmptyTexture();
}

OpenGLTexture::~OpenGLTexture() {
    ZPG_OPENGL_CALL(glDeleteTextures(1, &m_RendererID));
}

void OpenGLTexture::LoadTexture(const std::string& path) {
    i32 width;
    i32 height;
    i32 channels;
    byte* imageData = nullptr;

    stbi_set_flip_vertically_on_load(true);
    imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);
    stbi_set_flip_vertically_on_load(false);

    ZPG_CORE_ASSERT(imageData != nullptr, "Failed to load image ({})!", path.c_str());

    m_Width = width;
    m_Height = height; 
    m_SampleSize = channels;

    m_DataFormat.Format = [channels, path]{
        switch (channels) {
            case 1: return BufferDataFormat::R8; 
            case 2: return BufferDataFormat::RG8; 
            case 3: return BufferDataFormat::RGB8;
            case 4: return BufferDataFormat::RGBA8;
            default: 
                ZPG_UNREACHABLE("Unknown texture format (number of channels) for image '{}'!", path.c_str()); 
                return BufferDataFormat::RGBA8;
        }
    }();

    CreateEmptyTexture();
    SetData(imageData, m_Width * m_Height * channels);

    stbi_image_free(imageData);
}

void OpenGLTexture::CreateEmptyTexture() {
    auto gl = BufferDataFormat::ToGL(m_DataFormat.Format);

    ZPG_OPENGL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
    ZPG_OPENGL_CALL(glTextureStorage2D(m_RendererID, 1, gl.InternalFormat, m_Width, m_Height));

    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));

    ZPG_CORE_INFO("Image ({}) size in bytes: {} ({}x{}x{})", 
                    m_Name, 
                    m_Width * m_Height * m_SampleSize, 
                    m_Width, m_Height, m_SampleSize);
}

void OpenGLTexture::Bind() {
    ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID)); 
}

void OpenGLTexture::BindToSlot(u32 slotIndex) {
    ZPG_OPENGL_CALL(glBindTextureUnit(slotIndex, m_RendererID));
}

void OpenGLTexture::Unbind() {
    // tf
    // ZPG_OPENGL_CALL(glBindTextureUnit(0, m_RendererID));
    ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

const std::string& OpenGLTexture::GetName() const {
    return m_Name;
}

u32 OpenGLTexture::GetWidth() const {
    return m_Width;
}

u32 OpenGLTexture::GetHeight() const {
    return m_Height;
}

void OpenGLTexture::SetData(const void *data, u32 size) {
    ZPG_CORE_ASSERT(size == (m_Width * m_Height * m_SampleSize), 
                    "Size ({} bytes) doesn't match the size of the texture ({} bytes = {}x{}x{})!", 
                    size, m_Width * m_Height * m_SampleSize, 
                    m_Width, m_Height, m_SampleSize);
   
    auto gl = BufferDataFormat::ToGL(m_DataFormat.Format);

    // GLuint texture, 
    // GLint level, 
    // GLint xoffset, GLint yoffset, 
    // GLsizei width, GLsizei height, 
    // GLenum format, GLenum type, 
    // const void *pixels

    ZPG_OPENGL_CALL(glTextureSubImage2D(
        m_RendererID, 
        0, 
        0, 0, 
        m_Width, m_Height, 
        gl.Format, gl.SampleType, 
        data));
}


void OpenGLTexture::AttachToFrameBuffer(
    u32 frameBufferID, 
    AttachmentType::Type attachmentType, 
    u32 index
) {
    auto gl = AttachmentType::ToGL(attachmentType);

    // TODO: Maybe this doesn't need to bind, because its named attaching
    
    Bind();

    glFramebufferTexture2D(
        GL_FRAMEBUFFER, 
        gl.Attachment + index,
        GL_TEXTURE_2D, 
        m_RendererID,
        0);

}

}
