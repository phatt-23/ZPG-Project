#include "OpenGLTexture2D.h"
#include "Debug/Asserter.h"
#include "Core/Core.h"
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>

#include "OpenGLMapper.h"
#include "OpenGLCore.h"
#include "Buffer/FrameBufferAttachment.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath, TextureDataFormat dataFormat)
: m_DataFormat(dataFormat) {
    ZPG_PROFILE_FUNCTION();
    m_Name = filepath.substr(
        filepath.find_last_of("/\\") + 1, 
        filepath.find_last_of('.') - filepath.find_last_of("/\\") - 1);

    LoadTexture(filepath);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& name, const std::string& filepath, TextureDataFormat dataFormat)
: m_Name(name)
, m_DataFormat(dataFormat) {
    ZPG_PROFILE_FUNCTION();
    LoadTexture(filepath);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& name, u32 width, u32 height, TextureDataFormat dataFormat)
: m_Name(name) 
, m_Width(width)
, m_Height(height) 
, m_SampleSize(OpenGLMapper::ToGL(dataFormat).SampleTypeSize)
, m_DataFormat(dataFormat) {
    ZPG_PROFILE_FUNCTION();
    Invalidate();
}

OpenGLTexture2D::~OpenGLTexture2D() {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glDeleteTextures(1, &m_RendererID));
}

void OpenGLTexture2D::LoadTexture(const std::string& path) {
    ZPG_PROFILE_FUNCTION();
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

    m_DataFormat = [channels, path]{
        switch (channels) {
            case 1: return TextureDataFormat::R8;
            case 2: return TextureDataFormat::RG8;
            case 3: return TextureDataFormat::RGB8;
            case 4: return TextureDataFormat::RGBA8;
            default: 
                ZPG_UNREACHABLE("Unknown texture format (number of channels) for image '{}'!", path.c_str()); 
                return TextureDataFormat::RGBA8;
        }
    }();

    Invalidate();
    SetData(imageData, m_Width * m_Height * channels);

    stbi_image_free(imageData);
}

void OpenGLTexture2D::BindToSlot(u32 slotIndex) {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glBindTextureUnit(slotIndex, m_RendererID));
}

const std::string& OpenGLTexture2D::GetName() const {
    ZPG_PROFILE_FUNCTION();
    return m_Name;
}

u32 OpenGLTexture2D::GetWidth() const {
    ZPG_PROFILE_FUNCTION();
    return m_Width;
}

u32 OpenGLTexture2D::GetHeight() const {
    ZPG_PROFILE_FUNCTION();
    return m_Height;
}

void OpenGLTexture2D::SetData(const void *data, u32 size) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(size == (m_Width * m_Height * m_SampleSize),
                    "Size ({} bytes) doesn't match the size of the texture ({} bytes = {}x{}x{})!", 
                    size, m_Width * m_Height * m_SampleSize, 
                    m_Width, m_Height, m_SampleSize);

    OpenGLMapper::OpenGLFormatMapping gl = OpenGLMapper::ToGL(m_DataFormat);

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


void OpenGLTexture2D::AttachToFrameBuffer(u32 frameBufferRendererID, const FrameBufferAttachment& frameBufferAttachment)
{
    ZPG_PROFILE_FUNCTION();
    OpenGLMapper::OpenGLAttachmentMapping gl = OpenGLMapper::ToGL(frameBufferAttachment.AttachType);

    // DSA
    ZPG_OPENGL_CALL(glNamedFramebufferTexture(
        frameBufferRendererID,
        gl.Attachment + frameBufferAttachment.Index,
        m_RendererID,
        0));
}

void OpenGLTexture2D::Resize(u32 width, u32 height) {
    ZPG_PROFILE_FUNCTION();

    m_Width = width;
    m_Height = height;
    Invalidate();
}

u32 OpenGLTexture2D::GetRendererID() const {
    ZPG_PROFILE_FUNCTION();
    return m_RendererID;
}

void OpenGLTexture2D::Invalidate()
{
    if (m_RendererID != 0) {
        ZPG_OPENGL_CALL(glDeleteTextures(1, &m_RendererID));
    }

    auto gl = OpenGLMapper::ToGL(m_DataFormat);

    ZPG_OPENGL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));

    ZPG_OPENGL_CALL(glTextureStorage2D(m_RendererID, 1, gl.InternalFormat, m_Width, m_Height));

    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));


    if (m_DataFormat == TextureDataFormat::Depth32F || m_DataFormat == TextureDataFormat::R8 || m_DataFormat == TextureDataFormat::RedInteger)
    {
        // Texture must be bound first
        GLint swizzle[4] = {
            GL_RED,   // Shader Red   channel source = Texture Red
            GL_RED,   // Shader Green channel source = Texture Red
            GL_RED,   // Shader Blue  channel source = Texture Red
            GL_ONE    // Shader Alpha channel source = One
        };
        ZPG_OPENGL_CALL(glTextureParameteriv(m_RendererID, GL_TEXTURE_SWIZZLE_RGBA, swizzle));
    }

    ZPG_CORE_INFO("Image ({}) size in bytes: {} ({}x{}x{})",
                    m_Name,
                    m_Width * m_Height * m_SampleSize,
                    m_Width, m_Height, m_SampleSize);
}

}
