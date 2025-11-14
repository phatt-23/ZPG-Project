//
// Created by phatt on 10/31/25.
//

#include "OpenGLMapper.h"

#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"


namespace ZPG {


OpenGLMapper::OpenGLAttachmentMapping OpenGLMapper::ToGL(const FrameBufferAttachmentType& bufferType) {
    ZPG_PROFILE_FUNCTION();
    switch (bufferType) {
        case FrameBufferAttachmentType::Color:         return { GL_COLOR_ATTACHMENT0, GL_COLOR_BUFFER_BIT };
        case FrameBufferAttachmentType::DepthStencil:  return { GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT };
        case FrameBufferAttachmentType::Depth:         return { GL_DEPTH_ATTACHMENT, GL_DEPTH_BUFFER_BIT };
        case FrameBufferAttachmentType::Stencil:       return { GL_STENCIL_ATTACHMENT, GL_STENCIL_BUFFER_BIT };
    }

    ZPG_UNREACHABLE();
    return {};
}

OpenGLMapper::OpenGLFormatMapping OpenGLMapper::ToGL(const TextureDataFormat& format) {
    ZPG_PROFILE_FUNCTION();
    switch (format) {
        case TextureDataFormat::R8:                 return { GL_R8,                 GL_RED,             GL_UNSIGNED_BYTE,       1 };
        case TextureDataFormat::RG8:                return { GL_RG8,                GL_RG,              GL_UNSIGNED_BYTE,       2 };
        case TextureDataFormat::RGB8:               return { GL_RGB8,               GL_RGB,             GL_UNSIGNED_BYTE,       3 };
        case TextureDataFormat::RGB16F:             return { GL_RGB16F,             GL_RGB,             GL_FLOAT,               6 };
        case TextureDataFormat::RGB32F:             return { GL_RGB32F,             GL_RGB,             GL_FLOAT,               12 };
        case TextureDataFormat::RGBA8:              return { GL_RGBA8,              GL_RGBA,            GL_UNSIGNED_BYTE,       4 };
        case TextureDataFormat::RGBA16F:            return { GL_RGBA16F,            GL_RGBA,            GL_FLOAT,               8 };
        case TextureDataFormat::RGBA32F:            return { GL_RGBA32F,            GL_RGBA,            GL_FLOAT,               16 };
        case TextureDataFormat::Depth24Stencil8:    return { GL_DEPTH24_STENCIL8,   GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8,   4 };
        case TextureDataFormat::Depth32F:           return { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT,               4 };
        case TextureDataFormat::RedInteger:         return { GL_R32I,               GL_RED_INTEGER,     GL_INT,                 4 };
        default: return { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4 };
    }
}

}
